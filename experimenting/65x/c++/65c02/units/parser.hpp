// 65x instruction parser
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


#ifndef w65c02_parser_hpp
#define w65c02_parser_hpp

#include "65c02.hpp"
#include "processor.hpp"

namespace w65c02
{


class Parser
{
public:
    using Opcode_processing = void (*)(Parser&);

    Parser(Processor& p, Mem& m);
    void bra(Sbyte offset)
    {
        pc += offset;
    }
    void jmp(Address addr)
    {
        pc = addr;
    }
    Address program_counter()
    {
        return pc;
    }
    Address addr0reg()
    {
        return addr0;
    }
    Byte imm0reg()
    {
        return imm0;
    }
    Word wimm0reg()
    {
        return wimm0;
    }
    Byte bank0reg()
    {
        return bank[0];
    }
    Byte bank1reg()
    {
        return bank[1];
    }
    void next_instruction();

private:
    static Opcode_processing select[256];

    Processor& processor;
    Mem& mem;
    Address pc = 0;
    Address addr0 = 0;
    Byte imm0 = 0;
    Word wimm0 = 0;
    Byte bank[2] {};
    
    void read_addr()
    {
        addr0 = mem.read16(pc);
        pc += 2;
    }

    void read_dpaddr()
    {
        addr0 = Address(mem.read8(pc++));
    }
    
    void read_imm()
    {
        imm0 = mem.read8(pc++);
    }

    void read_wimm()
    {
        wimm0 = mem.read16(pc);
        pc += 2;
    }

    void read_offset()
    {
        read_imm();
        addr0 = pc + as_signed(imm0);
    }

    void read_woffset()
    {
        read_wimm();
        addr0 = pc + wimm0;
    }
    
    void read_bank(int n = 0)
    {
        bank[n] = mem.read8(pc++);
    }
    
    void read_laddr()
    {
        read_addr();
        read_bank();
    }

    static void adc_abs(Parser&);
    static void adc_absl(Parser&);
    static void adc_abslx(Parser&);
    static void adc_absx(Parser&);
    static void adc_absy(Parser&);
    static void adc_dp(Parser&);
    static void adc_dpi(Parser&);
    static void adc_dpil(Parser&);
    static void adc_dpily(Parser&);
    static void adc_dpiy(Parser&);
    static void adc_dpx(Parser&);
    static void adc_dpxi(Parser&);
    static void adc_imm(Parser&);
    static void adc_sr(Parser&);
    static void adc_sriy(Parser&);
    static void and_abs(Parser&);
    static void and_absl(Parser&);
    static void and_abslx(Parser&);
    static void and_absx(Parser&);
    static void and_absy(Parser&);
    static void and_dp(Parser&);
    static void and_dpi(Parser&);
    static void and_dpil(Parser&);
    static void and_dpily(Parser&);
    static void and_dpiy(Parser&);
    static void and_dpx(Parser&);
    static void and_dpxi(Parser&);
    static void and_imm(Parser&);
    static void and_sr(Parser&);
    static void and_sriy(Parser&);
    static void asl_acc(Parser&);
    static void asl_abs(Parser&);
    static void asl_absx(Parser&);
    static void asl_dp(Parser&);
    static void asl_dpx(Parser&);
    static void bcc(Parser&);
    static void bcs(Parser&);
    static void beq(Parser&);
    static void bit_abs(Parser&);
    static void bit_absx(Parser&);
    static void bit_dp(Parser&);
    static void bit_dpx(Parser&);
    static void bit_imm(Parser&);
    static void bmi(Parser&);
    static void bne(Parser&);
    static void bpl(Parser&);
    static void bra(Parser&);    
    static void brk(Parser&);
    static void brl(Parser&);
    static void bvc(Parser&);
    static void bvs(Parser&);
    static void clc(Parser&);
    static void cld(Parser&);
    static void cli(Parser&);    
    static void clv(Parser&);
    static void cmp_abs(Parser&);
    static void cmp_absl(Parser&);
    static void cmp_abslx(Parser&);
    static void cmp_absx(Parser&);
    static void cmp_absy(Parser&);
    static void cmp_dp(Parser&);
    static void cmp_dpi(Parser&);
    static void cmp_dpil(Parser&);
    static void cmp_dpily(Parser&);
    static void cmp_dpiy(Parser&);
    static void cmp_dpx(Parser&);
    static void cmp_dpxi(Parser&);
    static void cmp_imm(Parser&);
    static void cmp_sr(Parser&);
    static void cmp_sriy(Parser&);
    static void cop(Parser&);
    static void cpx_abs(Parser&);
    static void cpx_dp(Parser&);
    static void cpx_imm(Parser&);
    static void cpy_abs(Parser&);
    static void cpy_dp(Parser&);
    static void cpy_imm(Parser&);
    static void dec_acc(Parser&);
    static void dec_abs(Parser&);
    static void dec_absx(Parser&);
    static void dec_dp(Parser&);
    static void dec_dpx(Parser&);
    static void dex(Parser&);
    static void dey(Parser&);
    static void eor_abs(Parser&);
    static void eor_absl(Parser&);
    static void eor_abslx(Parser&);
    static void eor_absx(Parser&);
    static void eor_absy(Parser&);
    static void eor_dp(Parser&);
    static void eor_dpi(Parser&);
    static void eor_dpil(Parser&);
    static void eor_dpily(Parser&);
    static void eor_dpiy(Parser&);
    static void eor_dpx(Parser&);
    static void eor_dpxi(Parser&);
    static void eor_imm(Parser&);
    static void eor_sr(Parser&);
    static void eor_sriy(Parser&);
    static void inc_acc(Parser&);
    static void inc_abs(Parser&);
    static void inc_absx(Parser&);
    static void inc_dp(Parser&);
    static void inc_dpx(Parser&);
    static void inx(Parser&);
    static void iny(Parser&);
    static void jml(Parser&);
    static void jmli(Parser&);
    static void jmp_abs(Parser&);
    static void jmp_absi(Parser&);
    static void jmp_absxi(Parser&);
    static void jsl(Parser&);
    static void jsr_abs(Parser&);
    static void jsr_absxi(Parser&);
    static void lda_abs(Parser&);
    static void lda_absl(Parser&);
    static void lda_abslx(Parser&);
    static void lda_absx(Parser&);
    static void lda_absy(Parser&);
    static void lda_dp(Parser&);
    static void lda_dpi(Parser&);
    static void lda_dpil(Parser&);
    static void lda_dpily(Parser&);
    static void lda_dpiy(Parser&);
    static void lda_dpx(Parser&);
    static void lda_dpxi(Parser&);
    static void lda_imm(Parser&);
    static void lda_sr(Parser&);
    static void lda_sriy(Parser&);
    static void ldx_abs(Parser&);
    static void ldx_absy(Parser&);
    static void ldx_dp(Parser&);
    static void ldx_dpy(Parser&);
    static void ldx_imm(Parser&);
    static void ldy_abs(Parser&);
    static void ldy_absx(Parser&);
    static void ldy_dp(Parser&);
    static void ldy_dpx(Parser&);
    static void ldy_imm(Parser&);
    static void lsr_acc(Parser&);
    static void lsr_abs(Parser&);
    static void lsr_absx(Parser&);
    static void lsr_dp(Parser&);
    static void lsr_dpx(Parser&);    
    static void mvn(Parser&);
    static void mvp(Parser&);
    static void nop(Parser&);
    static void ora_abs(Parser&);
    static void ora_absl(Parser&);
    static void ora_abslx(Parser&);
    static void ora_absx(Parser&);
    static void ora_absy(Parser&);
    static void ora_dp(Parser&);
    static void ora_dpi(Parser&);
    static void ora_dpil(Parser&);
    static void ora_dpily(Parser&);
    static void ora_dpiy(Parser&);
    static void ora_dpx(Parser&);
    static void ora_dpxi(Parser&);
    static void ora_imm(Parser&);
    static void ora_sr(Parser&);
    static void ora_sriy(Parser&);
    static void pea(Parser&);
    static void pei(Parser&);
    static void per(Parser&);
    static void pha(Parser&);
    static void phb(Parser&);
    static void phd(Parser&);
    static void phk(Parser&);
    static void php(Parser&);
    static void phx(Parser&);
    static void phy(Parser&);
    static void pla(Parser&);
    static void plb(Parser&);
    static void pld(Parser&);
    static void plp(Parser&);
    static void plx(Parser&);
    static void ply(Parser&);
    static void rep(Parser&);
    static void rol_acc(Parser&);
    static void rol_abs(Parser&);
    static void rol_absx(Parser&);
    static void rol_dp(Parser&);
    static void rol_dpx(Parser&);
    static void ror_acc(Parser&);
    static void ror_abs(Parser&);
    static void ror_absx(Parser&);
    static void ror_dp(Parser&);
    static void ror_dpx(Parser&);
    static void rti(Parser&);
    static void rtl(Parser&);
    static void rts(Parser&);
    static void sbc_abs(Parser&);
    static void sbc_absl(Parser&);
    static void sbc_abslx(Parser&);
    static void sbc_absx(Parser&);
    static void sbc_absy(Parser&);
    static void sbc_dp(Parser&);
    static void sbc_dpi(Parser&);
    static void sbc_dpil(Parser&);
    static void sbc_dpily(Parser&);
    static void sbc_dpiy(Parser&);
    static void sbc_dpx(Parser&);
    static void sbc_dpxi(Parser&);
    static void sbc_imm(Parser&);
    static void sbc_sr(Parser&);
    static void sbc_sriy(Parser&);
    static void sec(Parser&);
    static void sed(Parser&);
    static void sei(Parser&);
    static void sep(Parser&);
    static void sta_abs(Parser&);
    static void sta_absl(Parser&);
    static void sta_abslx(Parser&);
    static void sta_absx(Parser&);
    static void sta_absy(Parser&);
    static void sta_dp(Parser&);
    static void sta_dpi(Parser&);
    static void sta_dpil(Parser&);
    static void sta_dpily(Parser&);
    static void sta_dpiy(Parser&);
    static void sta_dpx(Parser&);
    static void sta_dpxi(Parser&);
    static void sta_sr(Parser&);
    static void sta_sriy(Parser&);
    static void stp(Parser&);
    static void stx_abs(Parser&);
    static void stx_dp(Parser&);
    static void stx_dpy(Parser&);
    static void sty_abs(Parser&);
    static void sty_dp(Parser&);
    static void sty_dpx(Parser&);
    static void stz_abs(Parser&);
    static void stz_absx(Parser&);
    static void stz_dp(Parser&);
    static void stz_dpx(Parser&);
    static void tax(Parser&);
    static void tay(Parser&);
    static void tcd(Parser&);
    static void tcs(Parser&);
    static void tdc(Parser&);
    static void trb_abs(Parser&);
    static void trb_dp(Parser&);
    static void tsb_abs(Parser&);
    static void tsb_dp(Parser&);
    static void tsc(Parser&);
    static void tsx(Parser&);
    static void txa(Parser&);
    static void txs(Parser&);
    static void txy(Parser&);
    static void tya(Parser&);
    static void tyx(Parser&);
    static void wai(Parser&);
    static void wdm(Parser&);
    static void xba(Parser&);
    static void xce(Parser&);
};

}
#endif /* w65c02_parser_hpp */
