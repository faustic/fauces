// Instruction processing interface
//
// Created by Alejandro Castro García on 11 May 2023
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


#ifndef w65c02_processor_hpp
#define w65c02_processor_hpp

namespace w65c02
{

class Processor
{
public:
    virtual ~Processor() {}
    virtual void adc_abs() = 0;
    virtual void adc_absl() = 0;
    virtual void adc_abslx() = 0;
    virtual void adc_absx() = 0;
    virtual void adc_absy() = 0;
    virtual void adc_dp() = 0;
    virtual void adc_dpi() = 0;
    virtual void adc_dpil() = 0;
    virtual void adc_dpily() = 0;
    virtual void adc_dpiy() = 0;
    virtual void adc_dpx() = 0;
    virtual void adc_dpxi() = 0;
    virtual void adc_imm() = 0;
    virtual void adc_sr() = 0;
    virtual void adc_sriy() = 0;
    virtual void and_abs() = 0;
    virtual void and_absl() = 0;
    virtual void and_abslx() = 0;
    virtual void and_absx() = 0;
    virtual void and_absy() = 0;
    virtual void and_dp() = 0;
    virtual void and_dpi() = 0;
    virtual void and_dpil() = 0;
    virtual void and_dpily() = 0;
    virtual void and_dpiy() = 0;
    virtual void and_dpx() = 0;
    virtual void and_dpxi() = 0;
    virtual void and_imm() = 0;
    virtual void and_sr() = 0;
    virtual void and_sriy() = 0;
    virtual void asl_acc() = 0;
    virtual void asl_abs() = 0;
    virtual void asl_absx() = 0;
    virtual void asl_dp() = 0;
    virtual void asl_dpx() = 0;
    virtual void bcc() = 0;
    virtual void bcs() = 0;
    virtual void beq() = 0;
    virtual void bit_abs() = 0;
    virtual void bit_absx() = 0;
    virtual void bit_dp() = 0;
    virtual void bit_dpx() = 0;
    virtual void bit_imm() = 0;
    virtual void bmi() = 0;
    virtual void bne() = 0;
    virtual void bpl() = 0;
    virtual void bra() = 0;
    virtual void brk() = 0;
    virtual void brl() = 0;
    virtual void bvc() = 0;
    virtual void bvs() = 0;
    virtual void clc() = 0;
    virtual void cld() = 0;
    virtual void cli() = 0;
    virtual void clv() = 0;
    virtual void cmp_abs() = 0;
    virtual void cmp_absl() = 0;
    virtual void cmp_abslx() = 0;
    virtual void cmp_absx() = 0;
    virtual void cmp_absy() = 0;
    virtual void cmp_dp() = 0;
    virtual void cmp_dpi() = 0;
    virtual void cmp_dpil() = 0;
    virtual void cmp_dpily() = 0;
    virtual void cmp_dpiy() = 0;
    virtual void cmp_dpx() = 0;
    virtual void cmp_dpxi() = 0;
    virtual void cmp_imm() = 0;
    virtual void cmp_sr() = 0;
    virtual void cmp_sriy() = 0;
    virtual void cop() = 0;
    virtual void cpx_abs() = 0;
    virtual void cpx_dp() = 0;
    virtual void cpx_imm() = 0;
    virtual void cpy_abs() = 0;
    virtual void cpy_dp() = 0;
    virtual void cpy_imm() = 0;
    virtual void dec_acc() = 0;
    virtual void dec_abs() = 0;
    virtual void dec_absx() = 0;
    virtual void dec_dp() = 0;
    virtual void dec_dpx() = 0;
    virtual void dex() = 0;
    virtual void dey() = 0;
    virtual void eor_abs() = 0;
    virtual void eor_absl() = 0;
    virtual void eor_abslx() = 0;
    virtual void eor_absx() = 0;
    virtual void eor_absy() = 0;
    virtual void eor_dp() = 0;
    virtual void eor_dpi() = 0;
    virtual void eor_dpil() = 0;
    virtual void eor_dpily() = 0;
    virtual void eor_dpiy() = 0;
    virtual void eor_dpx() = 0;
    virtual void eor_dpxi() = 0;
    virtual void eor_imm() = 0;
    virtual void eor_sr() = 0;
    virtual void eor_sriy() = 0;
    virtual void inc_acc() = 0;
    virtual void inc_abs() = 0;
    virtual void inc_absx() = 0;
    virtual void inc_dp() = 0;
    virtual void inc_dpx() = 0;
    virtual void inx() = 0;
    virtual void iny() = 0;
    virtual void jml() = 0;
    virtual void jmli() = 0;
    virtual void jmp_abs() = 0;
    virtual void jmp_absi() = 0;
    virtual void jmp_absxi() = 0;
    virtual void jsl() = 0;
    virtual void jsr_abs() = 0;
    virtual void jsr_absxi() = 0;
    virtual void lda_abs() = 0;
    virtual void lda_absl() = 0;
    virtual void lda_abslx() = 0;
    virtual void lda_absx() = 0;
    virtual void lda_absy() = 0;
    virtual void lda_dp() = 0;
    virtual void lda_dpi() = 0;
    virtual void lda_dpil() = 0;
    virtual void lda_dpily() = 0;
    virtual void lda_dpiy() = 0;
    virtual void lda_dpx() = 0;
    virtual void lda_dpxi() = 0;
    virtual void lda_imm() = 0;
    virtual void lda_sr() = 0;
    virtual void lda_sriy() = 0;
    virtual void ldx_abs() = 0;
    virtual void ldx_absy() = 0;
    virtual void ldx_dp() = 0;
    virtual void ldx_dpy() = 0;
    virtual void ldx_imm() = 0;
    virtual void ldy_abs() = 0;
    virtual void ldy_absx() = 0;
    virtual void ldy_dp() = 0;
    virtual void ldy_dpx() = 0;
    virtual void ldy_imm() = 0;
    virtual void lsr_acc() = 0;
    virtual void lsr_abs() = 0;
    virtual void lsr_absx() = 0;
    virtual void lsr_dp() = 0;
    virtual void lsr_dpx() = 0;
    virtual void mvn() = 0;
    virtual void mvp() = 0;
    virtual void nop() = 0;
    virtual void ora_abs() = 0;
    virtual void ora_absl() = 0;
    virtual void ora_abslx() = 0;
    virtual void ora_absx() = 0;
    virtual void ora_absy() = 0;
    virtual void ora_dp() = 0;
    virtual void ora_dpi() = 0;
    virtual void ora_dpil() = 0;
    virtual void ora_dpily() = 0;
    virtual void ora_dpiy() = 0;
    virtual void ora_dpx() = 0;
    virtual void ora_dpxi() = 0;
    virtual void ora_imm() = 0;
    virtual void ora_sr() = 0;
    virtual void ora_sriy() = 0;
    virtual void pea() = 0;
    virtual void pei() = 0;
    virtual void per() = 0;
    virtual void pha() = 0;
    virtual void phb() = 0;
    virtual void phd() = 0;
    virtual void phk() = 0;
    virtual void php() = 0;
    virtual void phx() = 0;
    virtual void phy() = 0;
    virtual void pla() = 0;
    virtual void plb() = 0;
    virtual void pld() = 0;
    virtual void plp() = 0;
    virtual void plx() = 0;
    virtual void ply() = 0;
    virtual void rep() = 0;
    virtual void rol_acc() = 0;
    virtual void rol_abs() = 0;
    virtual void rol_absx() = 0;
    virtual void rol_dp() = 0;
    virtual void rol_dpx() = 0;
    virtual void ror_acc() = 0;
    virtual void ror_abs() = 0;
    virtual void ror_absx() = 0;
    virtual void ror_dp() = 0;
    virtual void ror_dpx() = 0;
    virtual void rti() = 0;
    virtual void rtl() = 0;
    virtual void rts() = 0;
    virtual void sbc_abs() = 0;
    virtual void sbc_absl() = 0;
    virtual void sbc_abslx() = 0;
    virtual void sbc_absx() = 0;
    virtual void sbc_absy() = 0;
    virtual void sbc_dp() = 0;
    virtual void sbc_dpi() = 0;
    virtual void sbc_dpil() = 0;
    virtual void sbc_dpily() = 0;
    virtual void sbc_dpiy() = 0;
    virtual void sbc_dpx() = 0;
    virtual void sbc_dpxi() = 0;
    virtual void sbc_imm() = 0;
    virtual void sbc_sr() = 0;
    virtual void sbc_sriy() = 0;
    virtual void sec() = 0;
    virtual void sed() = 0;
    virtual void sei() = 0;
    virtual void sep() = 0;
    virtual void sta_abs() = 0;
    virtual void sta_absl() = 0;
    virtual void sta_abslx() = 0;
    virtual void sta_absx() = 0;
    virtual void sta_absy() = 0;
    virtual void sta_dp() = 0;
    virtual void sta_dpi() = 0;
    virtual void sta_dpil() = 0;
    virtual void sta_dpily() = 0;
    virtual void sta_dpiy() = 0;
    virtual void sta_dpx() = 0;
    virtual void sta_dpxi() = 0;
    virtual void sta_sr() = 0;
    virtual void sta_sriy() = 0;
    virtual void stp() = 0;
    virtual void stx_abs() = 0;
    virtual void stx_dp() = 0;
    virtual void stx_dpy() = 0;
    virtual void sty_abs() = 0;
    virtual void sty_dp() = 0;
    virtual void sty_dpx() = 0;
    virtual void stz_abs() = 0;
    virtual void stz_absx() = 0;
    virtual void stz_dp() = 0;
    virtual void stz_dpx() = 0;
    virtual void tax() = 0;
    virtual void tay() = 0;
    virtual void tcd() = 0;
    virtual void tcs() = 0;
    virtual void tdc() = 0;
    virtual void trb_abs() = 0;
    virtual void trb_dp() = 0;
    virtual void tsb_abs() = 0;
    virtual void tsb_dp() = 0;
    virtual void tsc() = 0;
    virtual void tsx() = 0;
    virtual void txa() = 0;
    virtual void txs() = 0;
    virtual void txy() = 0;
    virtual void tya() = 0;
    virtual void tyx() = 0;
    virtual void wai() = 0;
    virtual void wdm() = 0;
    virtual void xba() = 0;
    virtual void xce() = 0;
};

}

#endif /* w65c02_processor_hpp */
