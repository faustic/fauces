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
        bbr(6, parser.program_counter() - 4);
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
        bbr(7, parser.program_counter() - 4);
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
    *this << "adc " << parser.addr0reg() << ",x";
}

void Disassembler::adc_absy()
{
    *this << "adc " << parser.addr0reg() << ",y";
}

void Disassembler::adc_dp()
{
    throw Disassembler_unimplemented();
}

void Disassembler::adc_dpi()
{
    *this << "adc (" << parser.dp0reg() << ")";
}

void Disassembler::adc_dpil()
{
    if (rockwell)
        rmb(6, parser.program_counter() - 2);
    else
        *this << "adc [" << parser.dp0reg() << "]";
}

void Disassembler::adc_dpily()
{
    if (rockwell)
        rmb(7, parser.program_counter() - 2);
    else
        *this << "adc [" << parser.dp0reg() << "],y";
}

void Disassembler::adc_dpiy()
{
    *this << "adc (" << parser.dp0reg() << "),y";
}

void Disassembler::adc_dpx()
{
    *this << "adc " << parser.dp0reg() << ",x";
}

void Disassembler::adc_dpxi()
{
    *this << "adc " << "(" << parser.dp0reg() << ",x)";
}

void Disassembler::adc_imm()
{
    *this << "adc #" << parser.imm0reg();
}

void Disassembler::adc_sr()
{
    *this << "adc " << parser.imm0reg() << ",s";
}

void Disassembler::adc_sriy()
{
    *this << "adc (" << parser.imm0reg() << ",s),y";
}

void Disassembler::and_abs()
{
    *this << "and " << parser.addr0reg();
}

void Disassembler::and_absl()
{
    if (rockwell)
        bbr(2, parser.program_counter() - 4);
    else
    {
        Long_address addr = parser.bank0reg();
        addr <<=16;
        addr |= parser.addr0reg();
        *this << "and " << addr;
    }
}

void Disassembler::and_abslx()
{
    if (rockwell)
        bbr(3, parser.program_counter() - 4);
    else
    {
        Long_address addr = parser.bank0reg();
        addr <<=16;
        addr |= parser.addr0reg();
        *this << "and " << addr << ",x";
    }
}

void Disassembler::and_absx()
{
    *this << "and " << parser.addr0reg() << ",x";
}

void Disassembler::and_absy()
{
    *this << "and " << parser.addr0reg() << ",y";
}

void Disassembler::and_dp()
{
    *this << "and " << parser.dp0reg();
}

void Disassembler::and_dpi()
{
    *this << "and (" << parser.dp0reg() << ")";
}

void Disassembler::and_dpil()
{
    if (rockwell)
        rmb(2, parser.program_counter() - 2);
    else
        *this << "and [" << parser.dp0reg() << "]";
}

void Disassembler::and_dpily()
{
    if (rockwell)
        rmb(3, parser.program_counter() - 2);
    else
        *this << "and [" << parser.dp0reg() << "],y";
}

void Disassembler::and_dpiy()
{
    *this << "and (" << parser.dp0reg() << "),y";
}

void Disassembler::and_dpx()
{
    *this << "and " << parser.dp0reg() << ",x";
}

void Disassembler::and_dpxi()
{
    *this << "and " << "(" << parser.dp0reg() << ",x)";
}

void Disassembler::and_imm()
{
    *this << "and #" << parser.imm0reg();
}

void Disassembler::and_sr()
{
    *this << "and " << parser.imm0reg() << ",s";
}

void Disassembler::and_sriy()
{
    *this << "and (" << parser.imm0reg() << ",s),y";
}

void Disassembler::asl_acc()
{
    *this << "asl a";
}

void Disassembler::asl_abs()
{
    *this << "asl " << parser.addr0reg();
}

void Disassembler::asl_absx()
{
    *this << "asl " << parser.addr0reg() << ",x";
}

void Disassembler::asl_dp()
{
    *this << "asl " << parser.dp0reg();
}

void Disassembler::asl_dpx()
{
    *this << "asl " << parser.dp0reg() << ",x";
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
    *this << "bit " << parser.addr0reg();
}

void Disassembler::bit_absx()
{
    *this << "bit " << parser.addr0reg() << ",x";
}

void Disassembler::bit_dp()
{
    *this << "bit " << parser.dp0reg();
}

void Disassembler::bit_dpx()
{
    *this << "bit " << parser.dp0reg() << ",x";
}

void Disassembler::bit_imm()
{
    throw Disassembler_unimplemented();
}

void Disassembler::bmi()
{
    *this << "bmi " << parser.addr0reg();
}

void Disassembler::bne()
{
    *this << "bne " << parser.addr0reg();
}

void Disassembler::bpl()
{
    *this << "bpl " << parser.addr0reg();
}

void Disassembler::bra()
{
    *this << "bra " << parser.addr0reg();
}

void Disassembler::brk()
{
    *this << "brk";
}

void Disassembler::brl()
{
    throw Disassembler_unimplemented();
}

void Disassembler::bvc()
{
    *this << "bvc " << parser.addr0reg();
}

void Disassembler::bvs()
{
    *this << "bvs " << parser.addr0reg();
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
    *this << "cli";
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
    *this << "cop";
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
    *this << "dec a";
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
    *this << "eor " << parser.addr0reg();
}

void Disassembler::eor_absl()
{
    if (rockwell)
        bbr(4, parser.program_counter() - 4);
    else
    {
        Long_address addr = parser.bank0reg();
        addr <<=16;
        addr |= parser.addr0reg();
        *this << "eor " << addr;
    }
}

void Disassembler::eor_abslx()
{
    if (rockwell)
        bbr(5, parser.program_counter() - 4);
    else
    {
        Long_address addr = parser.bank0reg();
        addr <<=16;
        addr |= parser.addr0reg();
        *this << "eor " << addr << ",x";
    }
}

void Disassembler::eor_absx()
{
    *this << "eor " << parser.addr0reg() << ",x";
}

void Disassembler::eor_absy()
{
    *this << "eor " << parser.addr0reg() << ",y";
}

void Disassembler::eor_dp()
{
    *this << "eor " << parser.dp0reg();
}

void Disassembler::eor_dpi()
{
    *this << "eor (" << parser.dp0reg() << ")";
}

void Disassembler::eor_dpil()
{
    if (rockwell)
        rmb(4, parser.program_counter() - 2);
    else
        *this << "eor [" << parser.dp0reg() << "]";
}

void Disassembler::eor_dpily()
{
    if (rockwell)
        rmb(5, parser.program_counter() - 2);
    else
        *this << "eor [" << parser.dp0reg() << "],y";
}

void Disassembler::eor_dpiy()
{
    *this << "eor (" << parser.dp0reg() << "),y";
}

void Disassembler::eor_dpx()
{
    *this << "eor " << parser.dp0reg() << ",x";
}

void Disassembler::eor_dpxi()
{
    *this << "eor " << "(" << parser.dp0reg() << ",x)";
}

void Disassembler::eor_imm()
{
    *this << "eor #" << parser.imm0reg();
}

void Disassembler::eor_sr()
{
    *this << "eor " << parser.imm0reg() << ",s";
}

void Disassembler::eor_sriy()
{
    *this << "eor (" << parser.imm0reg() << ",s),y";
}

void Disassembler::inc_acc()
{
    *this << "inc a";
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
    *this << "iny";
}

void Disassembler::jml()
{
    Long_address addr = parser.bank0reg();
    addr <<=16;
    addr |= parser.addr0reg();
    *this << "jml " << addr;
}

void Disassembler::jmli()
{
    throw Disassembler_unimplemented();
}

void Disassembler::jmp_abs()
{
    *this << "jsr " << parser.addr0reg();
}

void Disassembler::jmp_absi()
{
    *this << "jmp (" << parser.addr0reg() << ")";
}

void Disassembler::jmp_absxi()
{
    *this << "jmp (" << parser.addr0reg() << ",x)";
}

void Disassembler::jsl()
{
    Long_address addr = parser.bank0reg();
    addr <<=16;
    addr |= parser.addr0reg();
    *this << "jsl " << addr;
}

void Disassembler::jsr_abs()
{
    *this << "jsr " << parser.addr0reg();
}

void Disassembler::jsr_absxi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lda_abs()
{
    *this << "lda " << parser.addr0reg();
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
    *this << "lda " << parser.dp0reg();
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
    *this << "lda " << parser.dp0reg() << ",x";
}

void Disassembler::lda_dpxi()
{
    throw Disassembler_unimplemented();
}

void Disassembler::lda_imm()
{
    *this << "lda #" << parser.imm0reg();
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
    *this << "ldx #" << parser.imm0reg();
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
    *this << "ldy #" << parser.imm0reg();
}

void Disassembler::lsr_acc()
{
    *this << "lsr a";
}

void Disassembler::lsr_abs()
{
    *this << "lsr " << parser.addr0reg();
}

void Disassembler::lsr_absx()
{
    *this << "lsr " << parser.addr0reg() << ",x";
}

void Disassembler::lsr_dp()
{
    *this << "lsr " << parser.dp0reg();
}

void Disassembler::lsr_dpx()
{
    *this << "lsr " << parser.dp0reg() << ",x";
}

void Disassembler::mvn()
{
    Address pc = Long_address(parser.program_counter() - 2);
    Long_address dst = Long_address(mem.read8(pc++)) << 16;
    Long_address src = Long_address(mem.read8(pc)) << 16;
    *this << "mvn " << src << "," << dst;
}

void Disassembler::mvp()
{
    Address pc = Long_address(parser.program_counter() - 2);
    Long_address dst = Long_address(mem.read8(pc++)) << 16;
    Long_address src = Long_address(mem.read8(pc)) << 16;
    *this << "mvp " << src << "," << dst;
}

void Disassembler::nop()
{
    *this << "nop";
}

void Disassembler::ora_abs()
{
    *this << "ora " << parser.addr0reg();
}

void Disassembler::ora_absl()
{
    if (rockwell)
        bbr(0, parser.program_counter() - 4);
    else
    {
        Long_address addr = parser.bank0reg();
        addr <<=16;
        addr |= parser.addr0reg();
        *this << "ora " << addr;
    }
}

void Disassembler::ora_abslx()
{
    if (rockwell)
        bbr(1, parser.program_counter() - 4);
    else
    {
        Long_address addr = parser.bank0reg();
        addr <<=16;
        addr |= parser.addr0reg();
        *this << "ora " << addr << ",x";
    }
}

void Disassembler::ora_absx()
{
    *this << "ora " << parser.addr0reg() << ",x";
}

void Disassembler::ora_absy()
{
    *this << "ora " << parser.addr0reg() << ",y";
}

void Disassembler::ora_dp()
{
    *this << "ora " << parser.dp0reg();
}

void Disassembler::ora_dpi()
{
    *this << "ora (" << parser.dp0reg() << ")";
}

void Disassembler::ora_dpil()
{
    if (rockwell)
        rmb(0, parser.program_counter() - 2);
    else
        *this << "ora [" << parser.dp0reg() << "]";
}

void Disassembler::ora_dpily()
{
    if (rockwell)
        rmb(1, parser.program_counter() - 2);
    else
        *this << "ora [" << parser.dp0reg() << "],y";
}

void Disassembler::ora_dpiy()
{
    *this << "ora (" << parser.dp0reg() << "),y";
}

void Disassembler::ora_dpx()
{
    *this << "ora " << parser.dp0reg() << ",x";
}

void Disassembler::ora_dpxi()
{
    *this << "ora " << "(" << parser.dp0reg() << ",x)";
}

void Disassembler::ora_imm()
{
    *this << "ora #" << parser.imm0reg();
}

void Disassembler::ora_sr()
{
    *this << "ora " << parser.imm0reg() << ",s";
}

void Disassembler::ora_sriy()
{
    *this << "ora (" << parser.imm0reg() << ",s),y";
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
    *this << "per " << parser.addr0reg();
}

void Disassembler::pha()
{
    *this << "pha";
}

void Disassembler::phb()
{
    throw Disassembler_unimplemented();
}

void Disassembler::phd()
{
    *this << "phd";
}

void Disassembler::phk()
{
    *this << "phk";
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
    *this << "phy";
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
    *this << "pld";
}

void Disassembler::plp()
{
    *this << "plp";
}

void Disassembler::plx()
{
    *this << "plx";
}

void Disassembler::ply()
{
    *this << "ply";
}

void Disassembler::rep()
{
    throw Disassembler_unimplemented();
}

void Disassembler::rol_acc()
{
    *this << "rol a";
}

void Disassembler::rol_abs()
{
    *this << "rol " << parser.addr0reg();
}

void Disassembler::rol_absx()
{
    *this << "rol " << parser.addr0reg() << ",x";
}

void Disassembler::rol_dp()
{
    *this << "rol " << parser.dp0reg();
}

void Disassembler::rol_dpx()
{
    *this << "rol " << parser.dp0reg() << ",x";
}

void Disassembler::ror_acc()
{
    *this << "ror a";
}

void Disassembler::ror_abs()
{
    *this << "ror " << parser.addr0reg();
}

void Disassembler::ror_absx()
{
    *this << "ror " << parser.addr0reg() << ",x";
}

void Disassembler::ror_dp()
{
    *this << "ror " << parser.dp0reg();
}

void Disassembler::ror_dpx()
{
    *this << "ror " << parser.dp0reg() << ",x";
}

void Disassembler::rti()
{
    *this << "rti";
}

void Disassembler::rtl()
{
    *this << "rtl";
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
    *this << "sec";
}

void Disassembler::sed()
{
    throw Disassembler_unimplemented();
}

void Disassembler::sei()
{
    *this << "sei";
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
    *this << "sta " << parser.dp0reg();
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
    *this << "sta " << parser.dp0reg() << ",x";
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
    *this << "stx " << parser.dp0reg();
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
    *this << "stz " << parser.addr0reg() << ",x";
}

void Disassembler::stz_dp()
{
    *this << "stz " << parser.dp0reg();
}

void Disassembler::stz_dpx()
{
    *this << "stz " << parser.dp0reg() << ",x";
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
    *this << "tcd";
}

void Disassembler::tcs()
{
    *this << "tcs";
}

void Disassembler::tdc()
{
    *this << "tdc";
}

void Disassembler::trb_abs()
{
    *this << "trb " << parser.addr0reg();
}

void Disassembler::trb_dp()
{
    *this << "trb " << parser.dp0reg();
}

void Disassembler::tsb_abs()
{
    *this << "tsb " << parser.addr0reg();
}

void Disassembler::tsb_dp()
{
    *this << "tsb " << parser.dp0reg();
}

void Disassembler::tsc()
{
    *this << "tsc";
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
    *this << "wdm";
}

void Disassembler::xba()
{
    throw Disassembler_unimplemented();
}

void Disassembler::xce()
{
    throw Disassembler_unimplemented();
}

