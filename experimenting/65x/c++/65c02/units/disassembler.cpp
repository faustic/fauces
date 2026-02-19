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


#include "disassembler.hpp"

using namespace w65c02;
using std::setw;
using std::setfill;

void Disassembler::adc_abs()
{
    *this << "adc " << parser.addr0reg();
}

void Disassembler::adc_absl()
{
    if (rockwell)
    {
        Address addr = parser.program_counter() - 3;
        Byte dp = mem.read8(addr++);
        Sbyte label = as_signed(mem.read8(addr++));
        parser.jmp(addr);
        addr += label;
        *this << "bbr6 " << dp << "," << addr;
    }
    else
    {
        Long_address addr = parser.bank0reg();
        addr <<=16;
        addr |= parser.addr0reg();
        *this << "adc " << addr;
    }
}

void Disassembler::adc_abslx()
{
    if (rockwell)
    {
        Address addr = parser.program_counter() - 3;
        Byte dp = mem.read8(addr++);
        Sbyte label = as_signed(mem.read8(addr++));
        parser.jmp(addr);
        addr += label;
        *this << "bbr7 " << dp << "," << addr;
    }
    else
    {
        Long_address addr = parser.bank0reg();
        addr <<=16;
        addr |= parser.addr0reg();
        *this << "adc" << addr << ",x";
    }
}

void Disassembler::adc_absx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::adc_absy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::adc_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::adc_dpi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::adc_dpil()
{
    throw Disassembler_unimplemented();
}

void Disassembler::adc_dpily()
{
    throw Disassembler_unimplemented();
}

void Disassembler::adc_dpiy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::adc_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::adc_dpxi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::adc_imm()
{
    throw Disassembler_unimplemented();
}

void Disassembler::adc_sr()
{
    throw Disassembler_unimplemented();
}

void Disassembler::adc_sriy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::and_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::and_absl()
{
    throw Disassembler_unimplemented();
}

void Disassembler::and_abslx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::and_absx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::and_absy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::and_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::and_dpi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::and_dpil()
{
    throw Disassembler_unimplemented();
}

void Disassembler::and_dpily()
{
    throw Disassembler_unimplemented();
}

void Disassembler::and_dpiy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::and_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::and_dpxi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::and_imm()
{
    *this << "and " << parser.imm0reg();
}

void Disassembler::and_sr()
{
    throw Disassembler_unimplemented();
}

void Disassembler::and_sriy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::asl_acc()
{
    throw Disassembler_unimplemented();
}

void Disassembler::asl_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::asl_absx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::asl_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::asl_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::bcc()
{
    throw Disassembler_unimplemented();
}

void Disassembler::bcs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::beq()
{
    throw Disassembler_unimplemented();
}

void Disassembler::bit_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::bit_absx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::bit_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::bit_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::bit_imm()
{
    throw Disassembler_unimplemented();
}

void Disassembler::bmi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::bne()
{
    *this << "bne " << parser.addr0reg();
}

void Disassembler::bpl()
{
    throw Disassembler_unimplemented();
}

void Disassembler::bra()
{
    throw Disassembler_unimplemented();
}

void Disassembler::brk()
{
    throw Disassembler_unimplemented();
}

void Disassembler::brl()
{
    throw Disassembler_unimplemented();
}

void Disassembler::bvc()
{
    throw Disassembler_unimplemented();
}

void Disassembler::bvs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::clc()
{
    *this << "clc";
}

void Disassembler::cld()
{
    *this << "cld";
}

void Disassembler::cli()
{
    throw Disassembler_unimplemented();
}

void Disassembler::clv()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cmp_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cmp_absl()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cmp_abslx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cmp_absx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cmp_absy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cmp_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cmp_dpi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cmp_dpil()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cmp_dpily()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cmp_dpiy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cmp_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cmp_dpxi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cmp_imm()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cmp_sr()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cmp_sriy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cop()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cpx_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cpx_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cpx_imm()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cpy_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cpy_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::cpy_imm()
{
    throw Disassembler_unimplemented();
}

void Disassembler::dec_acc()
{
    throw Disassembler_unimplemented();
}

void Disassembler::dec_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::dec_absx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::dec_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::dec_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::dex()
{
    throw Disassembler_unimplemented();
}

void Disassembler::dey()
{
    throw Disassembler_unimplemented();
}

void Disassembler::eor_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::eor_absl()
{
    throw Disassembler_unimplemented();
}

void Disassembler::eor_abslx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::eor_absx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::eor_absy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::eor_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::eor_dpi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::eor_dpil()
{
    throw Disassembler_unimplemented();
}

void Disassembler::eor_dpily()
{
    throw Disassembler_unimplemented();
}

void Disassembler::eor_dpiy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::eor_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::eor_dpxi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::eor_imm()
{
    throw Disassembler_unimplemented();
}

void Disassembler::eor_sr()
{
    throw Disassembler_unimplemented();
}

void Disassembler::eor_sriy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::inc_acc()
{
    throw Disassembler_unimplemented();
}

void Disassembler::inc_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::inc_absx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::inc_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::inc_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::inx()
{
    *this << "inx";
}

void Disassembler::iny()
{
    throw Disassembler_unimplemented();
}

void Disassembler::jml()
{
    throw Disassembler_unimplemented();
}

void Disassembler::jmli()
{
    throw Disassembler_unimplemented();
}

void Disassembler::jmp_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::jmp_absi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::jmp_absxi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::jsl()
{
    throw Disassembler_unimplemented();
}

void Disassembler::jsr_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::jsr_absxi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lda_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lda_absl()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lda_abslx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lda_absx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lda_absy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lda_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lda_dpi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lda_dpil()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lda_dpily()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lda_dpiy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lda_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lda_dpxi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lda_imm()
{
    *this << "lda " << parser.imm0reg();
}

void Disassembler::lda_sr()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lda_sriy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ldx_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ldx_absy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ldx_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ldx_dpy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ldx_imm()
{
    *this << "ldx " << parser.imm0reg();
}

void Disassembler::ldy_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ldy_absx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ldy_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ldy_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ldy_imm()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lsr_acc()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lsr_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lsr_absx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lsr_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lsr_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::mvn()
{
    throw Disassembler_unimplemented();
}

void Disassembler::mvp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::nop()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ora_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ora_absl()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ora_abslx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ora_absx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ora_absy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ora_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ora_dpi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ora_dpil()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ora_dpily()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ora_dpiy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ora_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ora_dpxi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ora_imm()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ora_sr()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ora_sriy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::pea()
{
    throw Disassembler_unimplemented();
}

void Disassembler::pei()
{
    throw Disassembler_unimplemented();
}

void Disassembler::per()
{
    throw Disassembler_unimplemented();
}

void Disassembler::pha()
{
    throw Disassembler_unimplemented();
}

void Disassembler::phb()
{
    throw Disassembler_unimplemented();
}

void Disassembler::phd()
{
    throw Disassembler_unimplemented();
}

void Disassembler::phk()
{
    throw Disassembler_unimplemented();
}

void Disassembler::php()
{
    *this << "php";
}

void Disassembler::phx()
{
    *this << "phx";
}

void Disassembler::phy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::pla()
{
    *this << "pla";
}

void Disassembler::plb()
{
    throw Disassembler_unimplemented();
}

void Disassembler::pld()
{
    throw Disassembler_unimplemented();
}

void Disassembler::plp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::plx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ply()
{
    throw Disassembler_unimplemented();
}

void Disassembler::rep()
{
    throw Disassembler_unimplemented();
}

void Disassembler::rol_acc()
{
    throw Disassembler_unimplemented();
}

void Disassembler::rol_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::rol_absx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::rol_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::rol_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ror_acc()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ror_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ror_absx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ror_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::ror_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::rti()
{
    throw Disassembler_unimplemented();
}

void Disassembler::rtl()
{
    throw Disassembler_unimplemented();
}

void Disassembler::rts()
{
    *this << "rts";
}

void Disassembler::sbc_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sbc_absl()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sbc_abslx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sbc_absx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sbc_absy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sbc_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sbc_dpi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sbc_dpil()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sbc_dpily()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sbc_dpiy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sbc_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sbc_dpxi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sbc_imm()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sbc_sr()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sbc_sriy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sec()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sed()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sei()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sep()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sta_abs()
{
    *this << "sta " << parser.addr0reg();
}

void Disassembler::sta_absl()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sta_abslx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sta_absx()
{
    *this << "sta " << parser.addr0reg() << ",x";
}

void Disassembler::sta_absy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sta_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sta_dpi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sta_dpil()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sta_dpily()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sta_dpiy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sta_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sta_dpxi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sta_sr()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sta_sriy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::stp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::stx_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::stx_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::stx_dpy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sty_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sty_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sty_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::stz_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::stz_absx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::stz_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::stz_dpx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::tax()
{
    throw Disassembler_unimplemented();
}

void Disassembler::tay()
{
    throw Disassembler_unimplemented();
}

void Disassembler::tcd()
{
    throw Disassembler_unimplemented();
}

void Disassembler::tcs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::tdc()
{
    throw Disassembler_unimplemented();
}

void Disassembler::trb_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::trb_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::tsb_abs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::tsb_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::tsc()
{
    throw Disassembler_unimplemented();
}

void Disassembler::tsx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::txa()
{
    throw Disassembler_unimplemented();
}

void Disassembler::txs()
{
    throw Disassembler_unimplemented();
}

void Disassembler::txy()
{
    throw Disassembler_unimplemented();
}

void Disassembler::tya()
{
    throw Disassembler_unimplemented();
}

void Disassembler::tyx()
{
    throw Disassembler_unimplemented();
}

void Disassembler::wai()
{
    throw Disassembler_unimplemented();
}

void Disassembler::wdm()
{
    throw Disassembler_unimplemented();
}

void Disassembler::xba()
{
    throw Disassembler_unimplemented();
}

void Disassembler::xce()
{
    throw Disassembler_unimplemented();
}

