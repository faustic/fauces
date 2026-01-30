/*
Licensed under the MIT License

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

package com.faustic.cpu.wdc65x;

abstract interface Processor
{
    int adc_abs(int addr);
    int adc_abs_x(int addr);
    int adc_abs_y(int addr);
    int adc_abslong(int addr);
    int adc_abslong_x(int addr);
    int adc_dp(int addr);
    int adc_dp__(int addr);
    int adc_dp_x(int addr);
    int adc_dp_x__(int addr);
    int adc_dp___y(int addr);
    int adc_dp__long(int addr);
    int adc_dp__long_y(int addr);
    int adc_imm(int addr);
    int adc_sr(int addr);
    int adc_sr__y(int addr);
    int and_abs(int addr);
    int and_abs_x(int addr);
    int and_abs_y(int addr);
    int and_abslong(int addr);
    int and_abslong_x(int addr);
    int and_dp(int addr);
    int and_dp__(int addr);
    int and_dp_x(int addr);
    int and_dp_x__(int addr);
    int and_dp___y(int addr);
    int and_dp__long(int addr);
    int and_dp__long_y(int addr);
    int and_imm(int addr);
    int and_sr(int addr);
    int and_sr__y(int addr);
    int asl_abs(int addr);
    int asl_abs_x(int addr);
    int asl_acc(int addr);
    int asl_dp(int addr);
    int asl_dp_x(int addr);
    int bcc(int addr);
    int bcs(int addr);
    int beq(int addr);
    int bit_abs(int addr);
    int bit_abs_x(int addr);
    int bit_dp(int addr);
    int bit_dp_x(int addr);
    int bit_imm(int addr);
    int bmi(int addr);
    int bne(int addr);
    int bpl(int addr);
    int bra(int addr);
    int brk(int addr);
    int brl(int addr);
    int bvc(int addr);
    int bvs(int addr);
    int clc(int addr);
    int cld(int addr);
    int cli(int addr);
    int clv(int addr);
    int cmp_abs(int addr);
    int cmp_abs_x(int addr);
    int cmp_abs_y(int addr);
    int cmp_abslong(int addr);
    int cmp_abslong_x(int addr);
    int cmp_dp(int addr);
    int cmp_dp__(int addr);
    int cmp_dp_x(int addr);
    int cmp_dp_x__(int addr);
    int cmp_dp___y(int addr);
    int cmp_dp__long(int addr);
    int cmp_dp__long_y(int addr);
    int cmp_imm(int addr);
    int cmp_sr(int addr);
    int cmp_sr__y(int addr);
    int cop(int addr);
    int cpx_abs(int addr);
    int cpx_dp(int addr);
    int cpx_imm(int addr);
    int cpy_abs(int addr);
    int cpy_dp(int addr);
    int cpy_imm(int addr);
    int dec_abs(int addr);
    int dec_abs_x(int addr);
    int dec_acc(int addr);
    int dec_dp(int addr);
    int dec_dp_x(int addr);
    int dex(int addr);
    int dey(int addr);
    int eor_abs(int addr);
    int eor_abs_x(int addr);
    int eor_abs_y(int addr);
    int eor_abslong(int addr);
    int eor_abslong_x(int addr);
    int eor_dp(int addr);
    int eor_dp__(int addr);
    int eor_dp_x(int addr);
    int eor_dp_x__(int addr);
    int eor_dp___y(int addr);
    int eor_dp__long(int addr);
    int eor_dp__long_y(int addr);
    int eor_imm(int addr);
    int eor_sr(int addr);
    int eor_sr__y(int addr);
    int inc_abs(int addr);
    int inc_abs_x(int addr);
    int inc_acc(int addr);
    int inc_dp(int addr);
    int inc_dp_x(int addr);
    int inx(int addr);
    int iny(int addr);
    int jml(int addr);
    int jml__(int addr);
    int jmp_abs(int addr);
    int jmp_abs__(int addr);
    int jmp_abs_x__(int addr);
    int jsl(int addr);
    int jsr_abs(int addr);
    int jsr_abs_x__(int addr);
    int lda_abs(int addr);
    int lda_abs_x(int addr);
    int lda_abs_y(int addr);
    int lda_abslong(int addr);
    int lda_abslong_x(int addr);
    int lda_dp(int addr);
    int lda_dp__(int addr);
    int lda_dp_x(int addr);
    int lda_dp_x__(int addr);
    int lda_dp___y(int addr);
    int lda_dp__long(int addr);
    int lda_dp__long_y(int addr);
    int lda_imm(int addr);
    int lda_sr(int addr);
    int lda_sr__y(int addr);
    int ldx_abs(int addr);
    int ldx_abs_y(int addr);
    int ldx_dp(int addr);
    int ldx_dp_y(int addr);
    int ldx_imm(int addr);
    int ldy_abs(int addr);
    int ldy_abs_x(int addr);
    int ldy_dp(int addr);
    int ldy_dp_x(int addr);
    int ldy_imm(int addr);
    int lsr_abs(int addr);
    int lsr_abs_x(int addr);
    int lsr_acc(int addr);
    int lsr_dp(int addr);
    int lsr_dp_x(int addr);
    int mvn(int addr);
    int mvp(int addr);
    int nop(int addr);
    int ora_abs(int addr);
    int ora_abs_x(int addr);
    int ora_abs_y(int addr);
    int ora_abslong(int addr);
    int ora_abslong_x(int addr);
    int ora_dp(int addr);
    int ora_dp__(int addr);
    int ora_dp_x(int addr);
    int ora_dp_x__(int addr);
    int ora_dp___y(int addr);
    int ora_dp__long(int addr);
    int ora_dp__long_y(int addr);
    int ora_imm(int addr);
    int ora_sr(int addr);
    int ora_sr__y(int addr);
    int pea(int addr);
    int pei(int addr);
    int per(int addr);
    int pha(int addr);
    int phb(int addr);
    int phd(int addr);
    int phk(int addr);
    int php(int addr);
    int phx(int addr);
    int phy(int addr);
    int pla(int addr);
    int plb(int addr);
    int pld(int addr);
    int plp(int addr);
    int plx(int addr);
    int ply(int addr);
    int rep(int addr);
    int rol_abs(int addr);
    int rol_abs_x(int addr);
    int rol_acc(int addr);
    int rol_dp(int addr);
    int rol_dp_x(int addr);
    int ror_abs(int addr);
    int ror_abs_x(int addr);
    int ror_acc(int addr);
    int ror_dp(int addr);
    int ror_dp_x(int addr);
    int rti(int addr);
    int rtl(int addr);
    int rts(int addr);
    int sbc_abs(int addr);
    int sbc_abs_x(int addr);
    int sbc_abs_y(int addr);
    int sbc_abslong(int addr);
    int sbc_abslong_x(int addr);
    int sbc_dp(int addr);
    int sbc_dp__(int addr);
    int sbc_dp_x(int addr);
    int sbc_dp_x__(int addr);
    int sbc_dp___y(int addr);
    int sbc_dp__long(int addr);
    int sbc_dp__long_y(int addr);
    int sbc_imm(int addr);
    int sbc_sr(int addr);
    int sbc_sr__y(int addr);
    int sec(int addr);
    int sed(int addr);
    int sei(int addr);
    int sep(int addr);
    int sta_abs(int addr);
    int sta_abs_x(int addr);
    int sta_abs_y(int addr);
    int sta_abslong(int addr);
    int sta_abslong_x(int addr);
    int sta_dp(int addr);
    int sta_dp__(int addr);
    int sta_dp_x(int addr);
    int sta_dp_x__(int addr);
    int sta_dp___y(int addr);
    int sta_dp__long(int addr);
    int sta_dp__long_y(int addr);
    int sta_sr(int addr);
    int sta_sr__y(int addr);
    int stp(int addr);
    int stx_abs(int addr);
    int stx_dp(int addr);
    int stx_dp_y(int addr);
    int sty_abs(int addr);
    int sty_dp(int addr);
    int sty_dp_x(int addr);
    int stz_abs(int addr);
    int stz_abs_x(int addr);
    int stz_dp(int addr);
    int stz_dp_x(int addr);
    int tax(int addr);
    int tay(int addr);
    int tcd(int addr);
    int tcs(int addr);
    int tdc(int addr);
    int trb_abs(int addr);
    int trb_dp(int addr);
    int tsb_abs(int addr);
    int tsb_dp(int addr);
    int tsc(int addr);
    int tsx(int addr);
    int txa(int addr);
    int txs(int addr);
    int txy(int addr);
    int tya(int addr);
    int tyx(int addr);
    int wai(int addr);
    int wdm(int addr);
    int xba(int addr);
    int xce(int addr);
}
