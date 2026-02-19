// 65x disassembler
//
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


#ifndef w65c02_disassembler_hpp
#define w65c02_disassembler_hpp

#include "parser.hpp"
#include "processor.hpp"

#include <string>
#include <sstream>
#include <iomanip>

namespace w65c02
{

class Disassembler_unimplemented
{
    // Provisional class to detect unimplemented opcodes
    // In the end, all opcodes will be implemented.
};

class Disassembler : public Processor
{
public:
    Disassembler(Mem& mem, Address start, Word max_count, bool rockwell):
    mem (mem), max_count(max_count), parser(*this, mem)
    {
        buffer << std::hex;
        go(start, max_count);
    }
    void go(Address start_addr)
    {
        go(start_addr, max_count);
    }
    void go(Address addr, Word max_count)
    {
        this->max_count = max_count;
        parser.jmp(addr);
        for (Word count = 0; count < max_count; ++count)
        {
            addr = parser.program_counter();
            *this << addr << ": ";
            try
            {
                parser.next_instruction();
            }
            catch(Disassembler_unimplemented)
            {
                *this << mem.read8(addr) << " **************\n";
                break;
            }
            buffer << "\n";
        }
    }
    void flush()
    {
        buffer.str().clear();
    }
    std::string text() const
    {
        return buffer.str();
    }
    Disassembler& operator<<(const char* c)
    {
        buffer << c;
        return *this;
    }
    Disassembler& operator<<(Byte b)
    {
        buffer << "$" << std::setfill('0') << std::setw(2) << (unsigned)b;
        return *this;
    }
    Disassembler& operator<<(Address addr)
    {
        buffer << "$" << std::setfill('0') << std::setw(4) << (unsigned)addr;
        return *this;
    }

    Disassembler& operator<<(Long_address addr)
    {
        buffer << "$" << std::setfill('0') << std::setw(6) <<
            (unsigned long)addr;
        return *this;
    }

private:
    Mem& mem;
    std::stringstream buffer;
    Parser parser;
    Word max_count;
    bool rockwell = false;
    
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

inline std::ostream& operator<<(std::ostream& os, const Disassembler& d)
{
    os << d.text();
    return os;
}

} // namespace w65c02

#endif // w65c02_disassembler_hpp
