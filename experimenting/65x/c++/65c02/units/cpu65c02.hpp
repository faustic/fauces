// 65C02 emulator
//
// Created by Alejandro Castro García on 9 May 2023
/*
Licensed under the MIT License.
 
Copyright (c) Faustic Inferno SL
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#ifndef w65c02_cpu65c02_hpp
#define w65c02_cpu65c02_hpp

#include "65c02.hpp"
#include "parser.hpp"
#include "processor.hpp"

namespace w65c02
{

class Cpu65c02_error: public runtime_error
{
public:
    Cpu65c02_error(const string& what_arg): runtime_error(what_arg)
    {}
};

class Cpu65c02 : public Processor
{
public:
    Cpu65c02(Mem& mem, nanoseconds period = {}):
    mem(mem), period(period), parser(*this, mem)
    {}
    void turn_on()
    {
        if (work.joinable())
            throw Cpu65c02_error("Previous thread did not end");
        work_id = this_thread::get_id();
        vdd = true;
        work = thread(Cpu65c02::on_sequence, this);
    }
    
    void turn_off()
    {
        if (this_thread::get_id() != work_id)
            throw Cpu65c02_error("Turned off from the wrong thread");
        if (!work.joinable())
            throw Cpu65c02_error("Thread already ended or not started");
        vdd = false;
        work.join();
    }
    
    void irq_high()
    {
        irqline_low = false;
        update_attention();
    }
    
    void irq_low()
    {
        irqline_low = true;
        update_attention();
    }
    void nmi_high()
    {
        nmiline_high = true;
    }
    
    void nmi_low()
    {
        bool was_high = nmiline_high;
        nmiline_high = false;
        if (was_high)
        {
            nmi_sequence = true;
            update_attention();
        }
        update_attention();
    }
    void reset_high()
    {
        bool was_low = !resb_high;
        resb_high = true;
        if (was_low)
        {
            resb_sequence = true;
            update_attention();
        }
    }
    void reset_low()
    {
        resb_high = false;
    }
    
    void change_period(nanoseconds newperiod)
    {
        period = newperiod;
    }

    void fail_on_unimplemented()
    {
        must_fail_on_unimplemented = true;
    }
    void nop_on_unimplemented()
    {
        must_fail_on_unimplemented = false;
    }
    void rockwell_additions(bool enable)
    {
        rockwell = enable;
    }
    void pause_additions(bool enable)
    {
        pausable = enable;
    }
    Byte sreg()
    {
        return s;
    }
    void update_attention()
    {
        attention_needed =  resb_sequence || nmi_sequence ||
                            (irqline_low && !(p & i_mask));
    }
    
    
private:
    static constexpr Word c_bit = 0;
    static constexpr Word z_bit = 1;
    static constexpr Word i_bit = 2;
    static constexpr Word d_bit = 3;
    static constexpr Word b_bit = 4;
    static constexpr Word u_bit = 5;
    static constexpr Word v_bit = 6;
    static constexpr Word n_bit = 7;

    static constexpr Word c_mask = 1 << c_bit;
    static constexpr Word z_mask = 1 << z_bit;
    static constexpr Word i_mask = 1 << i_bit;
    static constexpr Word d_mask = 1 << d_bit;
    static constexpr Word b_mask = 1 << b_bit;
    static constexpr Word u_mask = 1 << u_bit;
    static constexpr Word v_mask = 1 << v_bit;
    static constexpr Word n_mask = 1 << n_bit;

    Byte a = 0;
    Byte x = 0;
    Byte y = 0;
    Byte p = 0;
    Byte s = 0;
    Address pc = 0;

    atomic<bool> vdd = false;
    atomic<bool> attention_needed = false;
    atomic<bool> resb_sequence = false;
    atomic<bool> resb_high = false;
    atomic<bool> nmi_sequence = false;
    atomic<bool> irqline_low = false;
    atomic<bool> nmiline_high = true;

    bool must_fail_on_unimplemented = false;
    bool rockwell = false;
    bool pausable = false;
    
    Mem& mem;
    Parser parser;
    thread work;
    thread::id work_id;
    nanoseconds period {};
    time_point<steady_clock> last_time;
    

    void attend()
    {
        if (resb_sequence)
        {
            nmi_sequence = false;
            resb_sequence = false;
            handle_interrupt(0xfffc, p);
        }
        else if(nmi_sequence)
        {
            nmi_sequence = false;
            handle_interrupt(0xfffa, p);
        }
        else if (irqline_low && !(p & i_mask))
            handle_interrupt(0xfffe, p & ~b_mask);
        update_attention();
    }
    
    nanoseconds wait_period()
    {
        using namespace std::chrono_literals;
        return period != 0ns ? period : 1us;
    }
    
    void cycles(unsigned n)
    {
        // This should add a delay as needed to resemble a CPU running at a
        // particular clock rate.
        using namespace std::chrono_literals;
        if (period != 0ns)
        {
            time_point<steady_clock> expected_time = last_time + n * period;
            time_point<steady_clock> current_time = steady_clock::now();
            while (current_time < expected_time && current_time >= last_time)
            {
                // The delay added here may not be accurate enough, but it is
                // better not to try optimisations until we have a real need
                // and some real system for comparison.
                this_thread::sleep_for(expected_time - current_time);
                current_time = steady_clock::now();
            }
            last_time = expected_time;
        }
    }
    
    void handle_interrupt(Address vector, Byte status)
    {
        push16v(parser.program_counter());
        pushv(status);
        p |= i_mask;
        p &= ~d_mask;
        parser.jmp(mem.read16(vector));
        cycles(7);
    }
    
    bool pages_differ(Address addr1, Address addr2)
    {
        return ((addr1 & 0xff00) != (addr2 & 0xff00));
    }
    
    Address pull()
    {
        return Address(0x100 + (++s & 0xff));
    }
    
    Byte pullv()
    {
        return mem.read8(pull());
    }
    
    Word pull16v()
    {
        Word value = Word(pullv());
        value |= Word(pullv()) << 8;
        return value;
    }
    
    Address push()
    {
        return 0x100 + (s-- & 0xff);
    }
    
    void pushv(Byte value)
    {
        mem.write8(push(), value);
    }
    
    Address push16()
    {
        --s;
        return push();
    }
    
    void push16v(Word value)
    {
        pushv(Byte(value >> 8));
        pushv(Byte(value));
    }
    
    void loop();
    static void on_sequence(Cpu65c02* cpu);
    
    void update_flags_n_z(Byte value)
    {
        p &= ~(n_mask | z_mask);
        if (value & 0x80)
            p |= n_mask;
        if (!value)
            p |= z_mask;
    }
    
    Byte adc_dec(Byte v1, Byte v2)
    {
        Byte c = (p & c_mask) >> c_bit;
        p &= ~c_mask;
        Byte p1 = v1 & 0xf;
        Byte p2 = v2 & 0xf;
        Byte result = p1 + p2 + c;
        c = 0;
        if (result >= 0x0a)
        {
            result = (result - 0xa) & 0xf;
            c = 0x10;
        }
        p1 = v1 & 0xf0;
        p2 = v2 & 0xf0;
        Word rt = Word(p1) + p2 + c;
        c = 0;
        if (rt >= 0xa0)
        {
            rt -= 0xa0;
            p |= c_mask;
        }
        result |= Byte(rt);
        p &= ~v_mask;
        Sbyte b1 = as_signed(v1);
        Sbyte b2 = as_signed(v2);
        Sbyte br = as_signed(result);
        if (b1 <= 0 && b2 <= 0)
        {
            if (br > 0 || br > b1 || br > b2)
                p |= v_mask;
        }
        else if (b1 >= 0 && b2 >= 0)
        {
            if (br < 0 || br < b1 || br < b2)
                p |= v_mask;
        }

        update_flags_n_z(result);
        return result;
    }
    
    Byte adc_bin(Byte v1, Byte v2)
    {
        Byte c = (p & c_mask) >> c_bit;
        Word result = v1 + v2 + c;
        p &= ~c_mask;
        if (result >= 0x100)
            p |= c_mask;
        Sword sresult = Sword(as_signed(v1)) + as_signed(v2) + as_signed(c);
        p &= ~v_mask;
        if ((sresult < 0) != (as_signed(Byte(result)) < 0))
            p |= v_mask;
        update_flags_n_z(result);
        return Byte(result);
    }
    
    Byte adc(Byte v1, Byte v2, bool uses_decimal)
    {
        return uses_decimal ? adc_dec(v1, v2) : adc_bin(v1, v2);
    }
    
    Byte asl(Byte value)
    {
        p &= ~c_mask;
        p |= (value >> (7 - c_bit)) & c_mask;
        value <<= 1;
        update_flags_n_z(value);
        return value;
    }
    
    void asl(Address addr)
    {
        Byte value = mem.read8(addr);
        mem.write8(addr, asl(value));
    }
    
    void bbr(Word bit) // 0 < bit <= 7
    {
        if (rockwell)
        {
            // The parser does not know about this instruction, so we do the
            // parsing here and then we update the parser program counter
            Byte value = mem.read8(Address(mem.read8(pc + 1)));
            if (!(value & (1 << bit)))
                parser.jmp(pc + 3 + as_signed(mem.read8(pc + 2)));
            else
                parser.jmp(pc + 3);
            cycles(5);
        }
        else
            unimplemented(1, 1);
    }
    
    void bbs(Word bit) // 0 < bit <= 7
    {
        if (rockwell)
        {
            // The parser does not know about this instruction, so we do the
            // parsing here and then we update the parser program counter
            Byte value = mem.read8(Address(mem.read8(pc + 1)));
            if (value & (1 << bit))
                parser.jmp(pc + 3 + as_signed(mem.read8(pc + 2)));
            else
                parser.jmp(pc + 3);
            cycles(5);
        }
        else
            unimplemented(1, 1);
    }
    
    void bit(Address addr)
    {
        Byte value = mem.read8(addr);
        p &= ~(n_mask | v_mask | z_mask);
        p |= value & (n_mask | v_mask);
        if (!(value & a))
            p |= z_mask;
    }
    
    void branch_taken()
    {
        Address regular_pc = parser.program_counter();
        parser.jmp(parser.addr0reg());
        cycles(1 + pages_differ(regular_pc, parser.program_counter()));
    }
    
    void cmp(Byte v1, Byte v2)
    {
        Byte result = v1 - v2;
        p &= ~c_mask;
        if (v1 >= v2)
            p |= c_mask; // C is set when no borrow
        update_flags_n_z(result);
    }
    
    void dec(Address addr)
    {
        Byte value = mem.read8(addr);
        mem.write8(addr, --value);
        update_flags_n_z(value);
    }
    
    void inc(Address addr)
    {
        Byte value = mem.read8(addr);
        mem.write8(addr, ++value);
        update_flags_n_z(value);
    }
    
    Byte lsr(Byte value)
    {
        p &= ~c_mask;
        p |= (value << c_bit) & c_mask;
        value >>= 1;
        update_flags_n_z(value);
        return value;
    }
    
    void lsr(Address addr)
    {
        Byte value = mem.read8(addr);
        mem.write8(addr, lsr(value));
    }
    
    void rmb(Word bit) // 0 < bit <= 7
    {
        if (rockwell)
        {
            // The parser does not know about this instruction, so we do the
            // parsing here and then we update the parser program counter
            Address addr = Address(mem.read8(pc + 1));
            Byte value = mem.read8(addr);
            mem.write8(addr, value & ~(1 << bit));
            parser.jmp(pc + 2);
            cycles(5);
        }
        else
            unimplemented(1, 1);
    }
    
    Byte rol(Byte value)
    {
        Byte rbit = (p & c_mask) >> c_bit;
        p &= ~c_mask;
        p |= (value & 0x80) >> (7 - c_bit);
        value <<= 1;
        value |= rbit;
        update_flags_n_z(value);
        return value;
    }
    
    void rol(Address addr)
    {
        Byte value = mem.read8(addr);
        mem.write8(addr, rol(value));
    }
    
    Byte ror(Byte value)
    {
        Byte lbit = (p & c_mask) << (7 - c_bit);
        p &= ~c_mask;
        p |= (value & 1) >> c_bit;
        value >>= 1;
        value |= lbit;
        update_flags_n_z(value);
        return value;
    }
    
    void ror(Address addr)
    {
        Byte value = mem.read8(addr);
        mem.write8(addr, ror(value));
    }
    
    Byte sbc_dec(Byte v1, Byte v2)
    {
        Byte c = !((p & c_mask) >> c_bit);
        sub_overflow(v1, v2, c);
        p |= c_mask;
        Byte p1 = v1 & 0xf;
        Byte p2 = v2 & 0xf;
        Byte result = p1 - p2 - c;
        c = 0;
        Byte c_bad = 0;
        if (as_signed(result) < 0)
        {
            result += 0xa;
            c = 1;
            if (as_signed(result) < 0)
                c_bad = 1;
            result &= 0xf;
        }
        p1 = (v1 >> 4) & 0xf;
        p2 = (v2 >> 4) & 0xf;
        Byte rt = p1 - p2 - c;
        if (as_signed(rt) < 0)
        {
            rt += 0xa;
            p &= ~c_mask;
        }
        rt -= c_bad;
        result |= rt << 4;
        update_flags_n_z(result);
        return result;
    }
    
    Byte sbc_bin(Byte v1, Byte v2)
    {
        Byte c = !((p & c_mask) >> c_bit);
        Word result = v1 - v2 - c;
        p |= c_mask;
        if (as_signed(result) < 0)
            p &= ~c_mask;
        sub_overflow(v1, v2, c);
        update_flags_n_z(result);
        return Byte(result);
    }
    
    Byte sbc(Byte v1, Byte v2, bool uses_decimal)
    {
        return uses_decimal ? sbc_dec(v1, v2) : sbc_bin(v1, v2);
    }
    
    void sub_overflow(Byte v1, Byte v2, Byte c)
    {
        p &= ~v_mask;
        Sword result = Sword(as_signed(v1)) - Sword(as_signed(v2)) - c;
        Sbyte br = as_signed(Byte(result));
        if ((br < 0) != (result < 0))
            p |= v_mask;
    }
    
    void smb(Word bit) // 0 < bit <= 7
    {
        if (rockwell)
        {
            // The parser does not know about this instruction, so we do the
            // parsing here and then we update the parser program counter
            Address addr = Address(mem.read8(pc + 1));
            Byte value = mem.read8(addr);
            mem.write8(addr, value | (1 << bit));
            parser.jmp(pc + 2);
            cycles(5);
        }
        else
            unimplemented(1, 1);
    }
    
    void trb(Address addr)
    {
        Byte value = mem.read8(addr);
        p &= ~z_mask;
        if (!(value & a))
            p |= z_mask;
        mem.write8(addr, value & ~a);
    }
    
    void tsb(Address addr)
    {
        Byte value = mem.read8(addr);
        p &= ~z_mask;
        if (!(value & a))
            p |= z_mask;
        mem.write8(addr, value | a);
    }
    
    void unimplemented_error();

    void unimplemented(Word numbytes, Word numcycles)
    {
        unimplemented_error();
        parser.jmp(pc + numbytes);
        cycles(numcycles);
    }
    
    void adc_abs() override;
    void adc_absl() override;
    void adc_abslx() override;
    void adc_absx() override;
    void adc_absy() override;
    void adc_dp() override;
    void adc_dpi() override;
    void adc_dpil() override;
    void adc_dpily() override;
    void adc_dpiy() override;
    void adc_dpx() override;
    void adc_dpxi() override;
    void adc_imm() override;
    void adc_sr() override;
    void adc_sriy() override;
    void and_abs() override;
    void and_absl() override;
    void and_abslx() override;
    void and_absx() override;
    void and_absy() override;
    void and_dp() override;
    void and_dpi() override;
    void and_dpil() override;
    void and_dpily() override;
    void and_dpiy() override;
    void and_dpx() override;
    void and_dpxi() override;
    void and_imm() override;
    void and_sr() override;
    void and_sriy() override;
    void asl_acc() override;
    void asl_abs() override;
    void asl_absx() override;
    void asl_dp() override;
    void asl_dpx() override;
    void bcc() override;
    void bcs() override;
    void beq() override;
    void bit_abs() override;
    void bit_absx() override;
    void bit_dp() override;
    void bit_dpx() override;
    void bit_imm() override;
    void bmi() override;
    void bne() override;
    void bpl() override;
    void bra() override;
    void brk() override;
    void brl() override;
    void bvc() override;
    void bvs() override;
    void clc() override;
    void cld() override;
    void cli() override;
    void clv() override;
    void cmp_abs() override;
    void cmp_absl() override;
    void cmp_abslx() override;
    void cmp_absx() override;
    void cmp_absy() override;
    void cmp_dp() override;
    void cmp_dpi() override;
    void cmp_dpil() override;
    void cmp_dpily() override;
    void cmp_dpiy() override;
    void cmp_dpx() override;
    void cmp_dpxi() override;
    void cmp_imm() override;
    void cmp_sr() override;
    void cmp_sriy() override;
    void cop() override;
    void cpx_abs() override;
    void cpx_dp() override;
    void cpx_imm() override;
    void cpy_abs() override;
    void cpy_dp() override;
    void cpy_imm() override;
    void dec_acc() override;
    void dec_abs() override;
    void dec_absx() override;
    void dec_dp() override;
    void dec_dpx() override;
    void dex() override;
    void dey() override;
    void eor_abs() override;
    void eor_absl() override;
    void eor_abslx() override;
    void eor_absx() override;
    void eor_absy() override;
    void eor_dp() override;
    void eor_dpi() override;
    void eor_dpil() override;
    void eor_dpily() override;
    void eor_dpiy() override;
    void eor_dpx() override;
    void eor_dpxi() override;
    void eor_imm() override;
    void eor_sr() override;
    void eor_sriy() override;
    void inc_acc() override;
    void inc_abs() override;
    void inc_absx() override;
    void inc_dp() override;
    void inc_dpx() override;
    void inx() override;
    void iny() override;
    void jml() override;
    void jmli() override;
    void jmp_abs() override;
    void jmp_absi() override;
    void jmp_absxi() override;
    void jsl() override;
    void jsr_abs() override;
    void jsr_absxi() override;
    void lda_abs() override;
    void lda_absl() override;
    void lda_abslx() override;
    void lda_absx() override;
    void lda_absy() override;
    void lda_dp() override;
    void lda_dpi() override;
    void lda_dpil() override;
    void lda_dpily() override;
    void lda_dpiy() override;
    void lda_dpx() override;
    void lda_dpxi() override;
    void lda_imm() override;
    void lda_sr() override;
    void lda_sriy() override;
    void ldx_abs() override;
    void ldx_absy() override;
    void ldx_dp() override;
    void ldx_dpy() override;
    void ldx_imm() override;
    void ldy_abs() override;
    void ldy_absx() override;
    void ldy_dp() override;
    void ldy_dpx() override;
    void ldy_imm() override;
    void lsr_acc() override;
    void lsr_abs() override;
    void lsr_absx() override;
    void lsr_dp() override;
    void lsr_dpx() override;
    void mvn() override;
    void mvp() override;
    void nop() override;
    void ora_abs() override;
    void ora_absl() override;
    void ora_abslx() override;
    void ora_absx() override;
    void ora_absy() override;
    void ora_dp() override;
    void ora_dpi() override;
    void ora_dpil() override;
    void ora_dpily() override;
    void ora_dpiy() override;
    void ora_dpx() override;
    void ora_dpxi() override;
    void ora_imm() override;
    void ora_sr() override;
    void ora_sriy() override;
    void pea() override;
    void pei() override;
    void per() override;
    void pha() override;
    void phb() override;
    void phd() override;
    void phk() override;
    void php() override;
    void phx() override;
    void phy() override;
    void pla() override;
    void plb() override;
    void pld() override;
    void plp() override;
    void plx() override;
    void ply() override;
    void rep() override;
    void rol_acc() override;
    void rol_abs() override;
    void rol_absx() override;
    void rol_dp() override;
    void rol_dpx() override;
    void ror_acc() override;
    void ror_abs() override;
    void ror_absx() override;
    void ror_dp() override;
    void ror_dpx() override;
    void rti() override;
    void rtl() override;
    void rts() override;
    void sbc_abs() override;
    void sbc_absl() override;
    void sbc_abslx() override;
    void sbc_absx() override;
    void sbc_absy() override;
    void sbc_dp() override;
    void sbc_dpi() override;
    void sbc_dpil() override;
    void sbc_dpily() override;
    void sbc_dpiy() override;
    void sbc_dpx() override;
    void sbc_dpxi() override;
    void sbc_imm() override;
    void sbc_sr() override;
    void sbc_sriy() override;
    void sec() override;
    void sed() override;
    void sei() override;
    void sep() override;
    void sta_abs() override;
    void sta_absl() override;
    void sta_abslx() override;
    void sta_absx() override;
    void sta_absy() override;
    void sta_dp() override;
    void sta_dpi() override;
    void sta_dpil() override;
    void sta_dpily() override;
    void sta_dpiy() override;
    void sta_dpx() override;
    void sta_dpxi() override;
    void sta_sr() override;
    void sta_sriy() override;
    void stp() override;
    void stx_abs() override;
    void stx_dp() override;
    void stx_dpy() override;
    void sty_abs() override;
    void sty_dp() override;
    void sty_dpx() override;
    void stz_abs() override;
    void stz_absx() override;
    void stz_dp() override;
    void stz_dpx() override;
    void tax() override;
    void tay() override;
    void tcd() override;
    void tcs() override;
    void tdc() override;
    void trb_abs() override;
    void trb_dp() override;
    void tsb_abs() override;
    void tsb_dp() override;
    void tsc() override;
    void tsx() override;
    void txa() override;
    void txs() override;
    void txy() override;
    void tya() override;
    void tyx() override;
    void wai() override;
    void wdm() override;
    void xba() override;
    void xce() override;    
};

}

#endif /* w65c02_cpu65c02_hpp */
