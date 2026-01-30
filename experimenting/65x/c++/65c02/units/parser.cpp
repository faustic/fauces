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


#include "parser.hpp"

w65c02::Parser::Opcode_processing w65c02::Parser::select[256] =
{
    brk, ora_dpxi, cop, ora_sr, //00-03
    tsb_dp, ora_dp, asl_dp, ora_dpil,
    php, ora_imm, asl_acc, phd,
    tsb_abs, ora_abs, asl_abs, ora_absl,
    
    bpl, ora_dpiy, ora_dpi, ora_sriy, //10-13
    trb_dp, ora_dpx, asl_dpx, ora_dpily,
    clc, ora_absy, inc_acc, tcs,
    trb_abs, ora_absx, asl_absx, ora_abslx,
    
    jsr_abs, and_dpxi, jsl, and_sr, //20-23
    bit_dp, and_dp, rol_dp, and_dpil,
    plp, and_imm, rol_acc, pld,
    bit_abs, and_abs, rol_abs, and_absl,
    
    bmi, and_dpiy, and_dpi, and_sriy, //30-33
    bit_dpx, and_dpx, rol_dpx, and_dpily,
    sec, and_absy, dec_acc, tsc,
    bit_absx, and_absx, rol_absx, and_abslx,
    
    rti, eor_dpxi, wdm, eor_sr, //40-43
    mvp, eor_dp, lsr_dp, eor_dpil,
    pha, eor_imm, lsr_acc, phk,
    jmp_abs, eor_abs, lsr_abs, eor_absl,
    
    bvc, eor_dpiy, eor_dpi, eor_sriy, //50-53
    mvn, eor_dpx, lsr_dpx, eor_dpily,
    cli, eor_absy, phy, tcd,
    jml, eor_absx, lsr_absx, eor_abslx,
    
    rts, adc_dpxi, per, adc_sr, //60-63
    stz_dp, adc_dp, ror_dp, adc_dpil,
    pla, adc_imm, ror_acc, rtl,
    jmp_absi, adc_abs, ror_abs, adc_absl,
    
    bvs, adc_dpiy, adc_dpi, adc_sriy, //70-73
    stz_dpx, adc_dpx, ror_dpx, adc_dpily,
    sei, adc_absy, ply, tdc,
    jmp_absxi, adc_absx, ror_absx, adc_abslx,

    bra, sta_dpxi, brl, sta_sr, //80-83
    sty_dp, sta_dp, stx_dp, sta_dpil,
    dey, bit_imm, txa, phb,
    sty_abs, sta_abs, stx_abs, sta_absl,
    
    bcc, sta_dpiy, sta_dpi, sta_sriy, //90-93
    sty_dpx, sta_dpx, stx_dpy, sta_dpily,
    tya, sta_absy, txs, txy,
    stz_abs, sta_absx, stz_absx, sta_abslx,
    
    ldy_imm, lda_dpxi, ldx_imm, lda_sr, //a0-a3
    ldy_dp, lda_dp, ldx_dp, lda_dpil,
    tay, lda_imm, tax, plb,
    ldy_abs, lda_abs, ldx_abs, lda_absl,
    
    bcs, lda_dpiy, lda_dpi, lda_sriy, //b0-b3
    ldy_dpx, lda_dpx, ldx_dpy, lda_dpily,
    clv, lda_absy, tsx, tyx,
    ldy_absx, lda_absx, ldx_absy, lda_abslx,
    
    cpy_imm, cmp_dpxi, rep, cmp_sr, //c0-c3
    cpy_dp, cmp_dp, dec_dp, cmp_dpil,
    iny, cmp_imm, dex, wai,
    cpy_abs, cmp_abs, dec_abs, cmp_absl,
    
    bne, cmp_dpiy, cmp_dpi, cmp_sriy, //d0-d3
    pei, cmp_dpx, dec_dpx, cmp_dpily,
    cld, cmp_absy, phx, stp,
    jmli, cmp_absx, dec_absx, cmp_abslx,
    
    cpx_imm, sbc_dpxi, sep, sbc_sr, //e0-e3
    cpx_dp, sbc_dp, inc_dp, sbc_dpil,
    inx, sbc_imm, nop, xba,
    cpx_abs, sbc_abs, inc_abs, sbc_absl,
    
    beq, sbc_dpiy, sbc_dpi, sbc_sriy, //f0-f3
    pea, sbc_dpx, inc_dpx, sbc_dpily,
    sed, sbc_absy, plx, xce,
    jsr_absxi, sbc_absx, inc_absx, sbc_abslx
};

w65c02::Parser::Parser(Processor& p, Mem& m): processor(p), mem(m)
{}

void w65c02::Parser::adc_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.adc_abs();
}

void w65c02::Parser::adc_absl(Parser & parser)
{
    parser.read_laddr();
    parser.processor.adc_absl();
}

void w65c02::Parser::adc_abslx(Parser & parser)
{
    parser.read_laddr();
    parser.processor.adc_abslx();
}

void w65c02::Parser::adc_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.adc_absx();
}

void w65c02::Parser::adc_absy(Parser & parser)
{
    parser.read_addr();
    parser.processor.adc_absy();
}

void w65c02::Parser::adc_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.adc_dp();
}

void w65c02::Parser::adc_dpi(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.adc_dpi();
}

void w65c02::Parser::adc_dpil(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.adc_dpil();
}

void w65c02::Parser::adc_dpily(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.adc_dpily();
}

void w65c02::Parser::adc_dpiy(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.adc_dpiy();
}

void w65c02::Parser::adc_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.adc_dpx();
}

void w65c02::Parser::adc_dpxi(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.adc_dpxi();
}

void w65c02::Parser::adc_imm(Parser & parser)
{
    parser.read_imm();
    parser.processor.adc_imm();
}

void w65c02::Parser::adc_sr(Parser & parser)
{
    parser.read_imm();
    parser.processor.adc_sr();
}

void w65c02::Parser::adc_sriy(Parser & parser)
{
    parser.read_imm();
    parser.processor.adc_sriy();
}

void w65c02::Parser::and_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.and_abs();
}

void w65c02::Parser::and_absl(Parser & parser)
{
    parser.read_laddr();
    parser.processor.and_absl();
}

void w65c02::Parser::and_abslx(Parser & parser)
{
    parser.read_laddr();
    parser.processor.and_abslx();
}

void w65c02::Parser::and_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.and_absx();
}

void w65c02::Parser::and_absy(Parser & parser)
{
    parser.read_addr();
    parser.processor.and_absy();
}

void w65c02::Parser::and_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.and_dp();
}

void w65c02::Parser::and_dpi(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.and_dpi();
}

void w65c02::Parser::and_dpil(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.and_dpil();
}

void w65c02::Parser::and_dpily(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.and_dpily();
}

void w65c02::Parser::and_dpiy(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.and_dpiy();
}

void w65c02::Parser::and_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.and_dpx();
}

void w65c02::Parser::and_dpxi(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.and_dpxi();
}

void w65c02::Parser::and_imm(Parser & parser)
{
    parser.read_imm();
    parser.processor.and_imm();
}

void w65c02::Parser::and_sr(Parser & parser)
{
    parser.read_imm();
    parser.processor.and_sr();
}

void w65c02::Parser::and_sriy(Parser & parser)
{
    parser.read_imm();
    parser.processor.and_sriy();
}

void w65c02::Parser::asl_acc(Parser & parser)
{
    parser.processor.asl_acc();
}

void w65c02::Parser::asl_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.asl_abs();
}

void w65c02::Parser::asl_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.asl_absx();
}

void w65c02::Parser::asl_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.asl_dp();
}

void w65c02::Parser::asl_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.asl_dpx();
}

void w65c02::Parser::bcc(Parser & parser)
{
    parser.read_offset();
    parser.processor.bcc();
}

void w65c02::Parser::bcs(Parser & parser)
{
    parser.read_offset();
    parser.processor.bcs();
}

void w65c02::Parser::beq(Parser & parser)
{
    parser.read_offset();
    parser.processor.beq();
}

void w65c02::Parser::bit_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.bit_abs();
}

void w65c02::Parser::bit_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.bit_absx();
}

void w65c02::Parser::bit_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.bit_dp();
}

void w65c02::Parser::bit_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.bit_dpx();
}

void w65c02::Parser::bit_imm(Parser & parser)
{
    parser.read_imm();
    parser.processor.bit_imm();
}

void w65c02::Parser::bmi(Parser & parser)
{
    parser.read_offset();
    parser.processor.bmi();
}

void w65c02::Parser::bne(Parser & parser)
{
    parser.read_offset();
    parser.processor.bne();
}

void w65c02::Parser::bpl(Parser & parser)
{
    parser.read_offset();
    parser.processor.bpl();
}

void w65c02::Parser::bra(Parser & parser)
{
    parser.read_offset();
    parser.processor.bra();
}

void w65c02::Parser::brk(Parser & parser)
{
    parser.read_imm();
    parser.processor.brk();
}

void w65c02::Parser::brl(Parser & parser)
{
    parser.read_woffset();
    parser.processor.brl();
}

void w65c02::Parser::bvc(Parser & parser)
{
    parser.read_offset();
    parser.processor.bvc();
}

void w65c02::Parser::bvs(Parser & parser)
{
    parser.read_offset();
    parser.processor.bvs();
}

void w65c02::Parser::clc(Parser & parser)
{
    parser.processor.clc();
}

void w65c02::Parser::cld(Parser & parser)
{
    parser.processor.cld();
}

void w65c02::Parser::cli(Parser & parser)
{
    parser.processor.cli();
}

void w65c02::Parser::clv(Parser & parser)
{
    parser.processor.clv();
}

void w65c02::Parser::cmp_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.cmp_abs();
}

void w65c02::Parser::cmp_absl(Parser & parser)
{
    parser.read_laddr();
    parser.processor.cmp_absl();
}

void w65c02::Parser::cmp_abslx(Parser & parser)
{
    parser.read_laddr();
    parser.processor.cmp_abslx();
}

void w65c02::Parser::cmp_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.cmp_absx();
}

void w65c02::Parser::cmp_absy(Parser & parser)
{
    parser.read_addr();
    parser.processor.cmp_absy();
}

void w65c02::Parser::cmp_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.cmp_dp();
}

void w65c02::Parser::cmp_dpi(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.cmp_dpi();
}

void w65c02::Parser::cmp_dpil(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.cmp_dpil();
}

void w65c02::Parser::cmp_dpily(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.cmp_dpily();
}

void w65c02::Parser::cmp_dpiy(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.cmp_dpiy();
}

void w65c02::Parser::cmp_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.cmp_dpx();
}

void w65c02::Parser::cmp_dpxi(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.cmp_dpxi();
}

void w65c02::Parser::cmp_imm(Parser & parser)
{
    parser.read_imm();
    parser.processor.cmp_imm();
}

void w65c02::Parser::cmp_sr(Parser & parser)
{
    parser.read_imm();
    parser.processor.cmp_sr();
}

void w65c02::Parser::cmp_sriy(Parser & parser)
{
    parser.read_imm();
    parser.processor.cmp_sriy();
}

void w65c02::Parser::cop(Parser & parser)
{
    parser.read_imm();
    parser.processor.cop();
}

void w65c02::Parser::cpx_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.cpx_abs();
}

void w65c02::Parser::cpx_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.cpx_dp();
}

void w65c02::Parser::cpx_imm(Parser & parser)
{
    parser.read_imm();
    parser.processor.cpx_imm();
}

void w65c02::Parser::cpy_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.cpy_abs();
}

void w65c02::Parser::cpy_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.cpy_dp();
}

void w65c02::Parser::cpy_imm(Parser & parser)
{
    parser.read_imm();
    parser.processor.cpy_imm();
}

void w65c02::Parser::dec_acc(Parser & parser)
{
    parser.processor.dec_acc();
}

void w65c02::Parser::dec_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.dec_abs();
}

void w65c02::Parser::dec_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.dec_absx();
}

void w65c02::Parser::dec_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.dec_dp();
}

void w65c02::Parser::dec_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.dec_dpx();
}

void w65c02::Parser::dex(Parser & parser)
{
    parser.processor.dex();
}

void w65c02::Parser::dey(Parser & parser)
{
    parser.processor.dey();
}

void w65c02::Parser::eor_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.eor_abs();
}

void w65c02::Parser::eor_absl(Parser & parser)
{
    parser.read_laddr();
    parser.processor.eor_absl();
}

void w65c02::Parser::eor_abslx(Parser & parser)
{
    parser.read_laddr();
    parser.processor.eor_abslx();
}

void w65c02::Parser::eor_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.eor_absx();
}

void w65c02::Parser::eor_absy(Parser & parser)
{
    parser.read_addr();
    parser.processor.eor_absy();
}

void w65c02::Parser::eor_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.eor_dp();
}

void w65c02::Parser::eor_dpi(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.eor_dpi();
}

void w65c02::Parser::eor_dpil(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.eor_dpil();
}

void w65c02::Parser::eor_dpily(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.eor_dpily();
}

void w65c02::Parser::eor_dpiy(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.eor_dpiy();
}

void w65c02::Parser::eor_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.eor_dpx();
}

void w65c02::Parser::eor_dpxi(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.eor_dpxi();
}

void w65c02::Parser::eor_imm(Parser & parser)
{
    parser.read_imm();
    parser.processor.eor_imm();
}

void w65c02::Parser::eor_sr(Parser & parser)
{
    parser.read_imm();
    parser.processor.eor_sr();
}

void w65c02::Parser::eor_sriy(Parser & parser)
{
    parser.read_imm();
    parser.processor.eor_sriy();
}

void w65c02::Parser::inc_acc(Parser & parser)
{
    parser.processor.inc_acc();
}

void w65c02::Parser::inc_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.inc_abs();
}

void w65c02::Parser::inc_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.inc_absx();
}

void w65c02::Parser::inc_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.inc_dp();
}

void w65c02::Parser::inc_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.inc_dpx();
}

void w65c02::Parser::inx(Parser & parser)
{
    parser.processor.inx();
}

void w65c02::Parser::iny(Parser & parser)
{
    parser.processor.iny();
}

void w65c02::Parser::jml(Parser & parser)
{
    parser.read_laddr();
    parser.processor.jml();
}

void w65c02::Parser::jmli(Parser & parser)
{
    parser.read_addr();
    parser.processor.jmli();
}

void w65c02::Parser::jmp_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.jmp_abs();
}

void w65c02::Parser::jmp_absi(Parser & parser)
{
    parser.read_addr();
    parser.processor.jmp_absi();
}

void w65c02::Parser::jmp_absxi(Parser & parser)
{
    parser.read_addr();
    parser.processor.jmp_absxi();
}

void w65c02::Parser::jsl(Parser & parser)
{
    parser.read_addr();
    parser.processor.jsl();
}

void w65c02::Parser::jsr_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.jsr_abs();
}

void w65c02::Parser::jsr_absxi(Parser & parser)
{
    parser.read_addr();
    parser.processor.jsr_absxi();
}

void w65c02::Parser::lda_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.lda_abs();
}

void w65c02::Parser::lda_absl(Parser & parser)
{
    parser.read_laddr();
    parser.processor.lda_absl();
}

void w65c02::Parser::lda_abslx(Parser & parser)
{
    parser.read_laddr();
    parser.processor.lda_abslx();
}

void w65c02::Parser::lda_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.lda_absx();
}

void w65c02::Parser::lda_absy(Parser & parser)
{
    parser.read_addr();
    parser.processor.lda_absy();
}

void w65c02::Parser::lda_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.lda_dp();
}

void w65c02::Parser::lda_dpi(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.lda_dpi();
}

void w65c02::Parser::lda_dpil(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.lda_dpil();
}

void w65c02::Parser::lda_dpily(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.lda_dpily();
}

void w65c02::Parser::lda_dpiy(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.lda_dpiy();
}

void w65c02::Parser::lda_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.lda_dpx();
}

void w65c02::Parser::lda_dpxi(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.lda_dpxi();
}

void w65c02::Parser::lda_imm(Parser & parser)
{
    parser.read_imm();
    parser.processor.lda_imm();
}

void w65c02::Parser::lda_sr(Parser & parser)
{
    parser.read_imm();
    parser.processor.lda_sr();
}

void w65c02::Parser::lda_sriy(Parser & parser)
{
    parser.read_imm();
    parser.processor.lda_sriy();
}

void w65c02::Parser::ldx_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.ldx_abs();
}

void w65c02::Parser::ldx_absy(Parser & parser)
{
    parser.read_addr();
    parser.processor.ldx_absy();
}

void w65c02::Parser::ldx_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.ldx_dp();
}

void w65c02::Parser::ldx_dpy(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.ldx_dpy();
}

void w65c02::Parser::ldx_imm(Parser & parser)
{
    parser.read_imm();
    parser.processor.ldx_imm();
}

void w65c02::Parser::ldy_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.ldy_abs();
}

void w65c02::Parser::ldy_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.ldy_absx();
}

void w65c02::Parser::ldy_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.ldy_dp();
}

void w65c02::Parser::ldy_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.ldy_dpx();
}

void w65c02::Parser::ldy_imm(Parser & parser)
{
    parser.read_imm();
    parser.processor.ldy_imm();
}

void w65c02::Parser::lsr_acc(Parser & parser)
{
    parser.processor.lsr_acc();
}

void w65c02::Parser::lsr_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.lsr_abs();
}

void w65c02::Parser::lsr_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.lsr_absx();
}

void w65c02::Parser::lsr_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.lsr_dp();
}

void w65c02::Parser::lsr_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.lsr_dpx();
}

void w65c02::Parser::mvn(Parser & parser)
{
    parser.read_bank(0);
    parser.read_bank(1);
    parser.processor.mvn();
}

void w65c02::Parser::mvp(Parser & parser)
{
    parser.read_bank(0);
    parser.read_bank(1);
    parser.processor.mvp();
}

void w65c02::Parser::nop(Parser & parser)
{
    parser.processor.nop();
}

void w65c02::Parser::ora_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.ora_abs();
}

void w65c02::Parser::ora_absl(Parser & parser)
{
    parser.read_laddr();
    parser.processor.ora_absl();
}

void w65c02::Parser::ora_abslx(Parser & parser)
{
    parser.read_laddr();
    parser.processor.ora_abslx();
}

void w65c02::Parser::ora_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.ora_absx();
}

void w65c02::Parser::ora_absy(Parser & parser)
{
    parser.read_addr();
    parser.processor.ora_absy();
}

void w65c02::Parser::ora_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.ora_dp();
}

void w65c02::Parser::ora_dpi(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.ora_dpi();
}

void w65c02::Parser::ora_dpil(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.ora_dpil();
}

void w65c02::Parser::ora_dpily(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.ora_dpily();
}

void w65c02::Parser::ora_dpiy(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.ora_dpiy();
}

void w65c02::Parser::ora_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.ora_dpx();
}

void w65c02::Parser::ora_dpxi(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.ora_dpxi();
}

void w65c02::Parser::ora_imm(Parser & parser)
{
    parser.read_imm();
    parser.processor.ora_imm();
}

void w65c02::Parser::ora_sr(Parser & parser)
{
    parser.read_imm();
    parser.processor.ora_sr();
}

void w65c02::Parser::ora_sriy(Parser & parser)
{
    parser.read_imm();
    parser.processor.ora_sriy();
}

void w65c02::Parser::pea(Parser & parser)
{
    parser.read_addr();
    parser.processor.pea();
}

void w65c02::Parser::pei(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.pei();
}

void w65c02::Parser::per(Parser & parser)
{
    parser.read_woffset();
    parser.processor.per();
}

void w65c02::Parser::pha(Parser & parser)
{
    parser.processor.pha();
}

void w65c02::Parser::phb(Parser & parser)
{
    parser.processor.phb();
}

void w65c02::Parser::phd(Parser & parser)
{
    parser.processor.phd();
}

void w65c02::Parser::phk(Parser & parser)
{
    parser.processor.phk();
}

void w65c02::Parser::php(Parser & parser)
{
    parser.processor.php();
}

void w65c02::Parser::phx(Parser & parser)
{
    parser.processor.phx();
}

void w65c02::Parser::phy(Parser & parser)
{
    parser.processor.phy();
}

void w65c02::Parser::pla(Parser & parser)
{
    parser.processor.pla();
}

void w65c02::Parser::plb(Parser & parser)
{
    parser.processor.plb();
}

void w65c02::Parser::pld(Parser & parser)
{
    parser.processor.pld();
}

void w65c02::Parser::plp(Parser & parser)
{
    parser.processor.plp();
}

void w65c02::Parser::plx(Parser & parser)
{
    parser.processor.plx();
}

void w65c02::Parser::ply(Parser & parser)
{
    parser.processor.ply();
}

void w65c02::Parser::rep(Parser & parser)
{
    parser.read_imm();
    parser.processor.rep();
}

void w65c02::Parser::rol_acc(Parser & parser)
{
    parser.processor.rol_acc();
}

void w65c02::Parser::rol_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.rol_abs();
}

void w65c02::Parser::rol_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.rol_absx();
}

void w65c02::Parser::rol_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.rol_dp();
}

void w65c02::Parser::rol_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.rol_dpx();
}

void w65c02::Parser::ror_acc(Parser & parser)
{
    parser.processor.ror_acc();
}

void w65c02::Parser::ror_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.ror_abs();
}

void w65c02::Parser::ror_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.ror_absx();
}

void w65c02::Parser::ror_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.ror_dp();
}

void w65c02::Parser::ror_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.ror_dpx();
}

void w65c02::Parser::rti(Parser & parser)
{
    parser.processor.rti();
}

void w65c02::Parser::rtl(Parser & parser)
{
    parser.processor.rtl();
}

void w65c02::Parser::rts(Parser & parser)
{
    parser.processor.rts();
}

void w65c02::Parser::sbc_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.sbc_abs();
}

void w65c02::Parser::sbc_absl(Parser & parser)
{
    parser.read_laddr();
    parser.processor.sbc_absl();
}

void w65c02::Parser::sbc_abslx(Parser & parser)
{
    parser.read_laddr();
    parser.processor.sbc_abslx();
}

void w65c02::Parser::sbc_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.sbc_absx();
}

void w65c02::Parser::sbc_absy(Parser & parser)
{
    parser.read_addr();
    parser.processor.sbc_absy();
}

void w65c02::Parser::sbc_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.sbc_dp();
}

void w65c02::Parser::sbc_dpi(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.sbc_dpi();
}

void w65c02::Parser::sbc_dpil(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.sbc_dpil();
}

void w65c02::Parser::sbc_dpily(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.sbc_dpily();
}

void w65c02::Parser::sbc_dpiy(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.sbc_dpiy();
}

void w65c02::Parser::sbc_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.sbc_dpx();
}

void w65c02::Parser::sbc_dpxi(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.sbc_dpxi();
}

void w65c02::Parser::sbc_imm(Parser & parser)
{
    parser.read_imm();
    parser.processor.sbc_imm();
}

void w65c02::Parser::sbc_sr(Parser & parser)
{
    parser.read_imm();
    parser.processor.sbc_sr();
}

void w65c02::Parser::sbc_sriy(Parser & parser)
{
    parser.read_imm();
    parser.processor.sbc_sriy();
}

void w65c02::Parser::sec(Parser & parser)
{
    parser.processor.sec();
}

void w65c02::Parser::sed(Parser & parser)
{
    parser.processor.sed();
}

void w65c02::Parser::sei(Parser & parser)
{
    parser.processor.sei();
}

void w65c02::Parser::sep(Parser & parser)
{
    parser.read_imm();
    parser.processor.sep();
}

void w65c02::Parser::sta_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.sta_abs();
}

void w65c02::Parser::sta_absl(Parser & parser)
{
    parser.read_laddr();
    parser.processor.sta_absl();
}

void w65c02::Parser::sta_abslx(Parser & parser)
{
    parser.read_laddr();
    parser.processor.sta_abslx();
}

void w65c02::Parser::sta_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.sta_absx();
}

void w65c02::Parser::sta_absy(Parser & parser)
{
    parser.read_addr();
    parser.processor.sta_absy();
}

void w65c02::Parser::sta_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.sta_dp();
}

void w65c02::Parser::sta_dpi(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.sta_dpi();
}

void w65c02::Parser::sta_dpil(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.sta_dpil();
}

void w65c02::Parser::sta_dpily(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.sta_dpily();
}

void w65c02::Parser::sta_dpiy(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.sta_dpiy();
}

void w65c02::Parser::sta_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.sta_dpx();
}

void w65c02::Parser::sta_dpxi(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.sta_dpxi();
}

void w65c02::Parser::sta_sr(Parser & parser)
{
    parser.read_imm();
    parser.processor.sta_sr();
}

void w65c02::Parser::sta_sriy(Parser & parser)
{
    parser.read_imm();
    parser.processor.sta_sriy();
}

void w65c02::Parser::stp(Parser & parser)
{
    parser.read_addr();
    parser.processor.stp();
}

void w65c02::Parser::stx_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.stx_abs();
}

void w65c02::Parser::stx_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.stx_dp();
}

void w65c02::Parser::stx_dpy(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.stx_dpy();
}

void w65c02::Parser::sty_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.sty_abs();
}

void w65c02::Parser::sty_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.sty_dp();
}

void w65c02::Parser::sty_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.sty_dpx();
}

void w65c02::Parser::stz_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.stz_abs();
}

void w65c02::Parser::stz_absx(Parser & parser)
{
    parser.read_addr();
    parser.processor.stz_absx();
}

void w65c02::Parser::stz_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.stz_dp();
}

void w65c02::Parser::stz_dpx(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.stz_dpx();
}

void w65c02::Parser::tax(Parser & parser)
{
    parser.processor.tax();
}

void w65c02::Parser::tay(Parser & parser)
{
    parser.processor.tay();
}

void w65c02::Parser::tcd(Parser & parser)
{
    parser.processor.tcd();
}

void w65c02::Parser::tcs(Parser & parser)
{
    parser.processor.tcs();
}

void w65c02::Parser::tdc(Parser & parser)
{
    parser.processor.tdc();
}

void w65c02::Parser::trb_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.trb_abs();
}

void w65c02::Parser::trb_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.trb_dp();
}

void w65c02::Parser::tsb_abs(Parser & parser)
{
    parser.read_addr();
    parser.processor.tsb_abs();
}

void w65c02::Parser::tsb_dp(Parser & parser)
{
    parser.read_dpaddr();
    parser.processor.tsb_dp();
}

void w65c02::Parser::tsc(Parser & parser)
{
    parser.processor.tsc();
}

void w65c02::Parser::tsx(Parser & parser)
{
    parser.processor.tsx();
}

void w65c02::Parser::txa(Parser & parser)
{
    parser.processor.txa();
}

void w65c02::Parser::txs(Parser & parser)
{
    parser.processor.txs();
}

void w65c02::Parser::txy(Parser & parser)
{
    parser.processor.txy();
}

void w65c02::Parser::tya(Parser & parser)
{
    parser.processor.tya();
}

void w65c02::Parser::tyx(Parser & parser)
{
    parser.processor.tyx();
}

void w65c02::Parser::wai(Parser & parser)
{
    parser.processor.wai();
}

void w65c02::Parser::wdm(Parser & parser)
{
    parser.read_imm();
    parser.processor.wdm();
}

void w65c02::Parser::xba(Parser & parser)
{
    parser.processor.xba();
}

void w65c02::Parser::xce(Parser & parser)
{
    parser.processor.xce();
}

void w65c02::Parser::next_instruction()
{
    Byte opcode = mem.read8(pc++);
    select[opcode](*this);
}

