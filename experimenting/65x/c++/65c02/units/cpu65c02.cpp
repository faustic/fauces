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


#include "cpu65c02.hpp"
#include <sstream>
#include <iomanip>

using std::hex;
using std::setw;
using std::setfill;

void w65c02::Cpu65c02::on_sequence(Cpu65c02* cpu)
{
    cpu->p |= u_mask | b_mask;
    while (!cpu->resb_sequence)
        this_thread::sleep_for(cpu->wait_period());
    cpu->loop();
}

void w65c02::Cpu65c02::loop()
{
    update_attention();
    last_time = steady_clock::now();
    while (vdd)
    {
        if (!resb_high)
            continue;
        if (attention_needed)
            attend();
        pc = parser.program_counter();
        parser.next_instruction();
    }
    a = 0;
    x = 0;
    y = 0;
    p = 0;
    s = 0;
    pc = 0;
}

void w65c02::Cpu65c02::unimplemented_error()
{
    if (must_fail_on_unimplemented)
    {
        std::stringstream stream;
        stream << "Unimplemented instruction at $" << hex << setfill('0');
        stream << setw(4) << (unsigned)pc << ": $";
        stream << setw(2) << (unsigned)mem.read8(pc);
        string msg = stream.str();
        throw Cpu65c02_error(msg);
    }
}

void w65c02::Cpu65c02::adc_abs()
{
    Byte b = mem.read8(parser.addr0reg());
    bool uses_decimal = p & d_mask;
    a = adc(a, b, uses_decimal);
    cycles(4 + uses_decimal);
}

void w65c02::Cpu65c02::adc_absl()
{
    bbr(6);
}

void w65c02::Cpu65c02::adc_abslx()
{
    bbr(7);
}

void w65c02::Cpu65c02::adc_absx()
{
    Address addr = parser.addr0reg() + x;
    Byte b = mem.read8(addr);
    bool uses_decimal = p & d_mask;
    a = adc(a, b, uses_decimal);
    cycles(4 + uses_decimal + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::adc_absy()
{
    Address addr = parser.addr0reg() + y;
    Byte b = mem.read8(addr);
    bool uses_decimal = p & d_mask;
    a = adc(a, b, uses_decimal);
    cycles(4 + uses_decimal + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::adc_dp()
{
    Byte b = mem.read8(parser.addr0reg());
    bool uses_decimal = p & d_mask;
    a = adc(a, b, uses_decimal);
    cycles(3 + uses_decimal);
}

void w65c02::Cpu65c02::adc_dpi()
{
    Byte b = mem.read8(Address(mem.read16(parser.addr0reg())));
    bool uses_decimal = p & d_mask;
    a = adc(a, b, uses_decimal);
    cycles(5 + uses_decimal);
}

void w65c02::Cpu65c02::adc_dpil()
{
    rmb(6);
}

void w65c02::Cpu65c02::adc_dpily()
{
    rmb(7);
}

void w65c02::Cpu65c02::adc_dpiy()
{
    Address addr = Address(mem.read16(parser.addr0reg()));
    Address addry = addr + y;
    Byte b = mem.read8(addry);
    bool uses_decimal = p & d_mask;
    a = adc(a, b, uses_decimal);
    cycles(5 + uses_decimal + pages_differ(addr, addry));
}

void w65c02::Cpu65c02::adc_dpx()
{
    Byte b = mem.read8((parser.addr0reg() + x) & 0xff);
    bool uses_decimal = p & d_mask;
    a = adc(a, b, uses_decimal);
    cycles(4 + uses_decimal);
}

void w65c02::Cpu65c02::adc_dpxi()
{
    Byte b = mem.read8(Address(mem.read16((parser.addr0reg() + x) & 0xff)));
    bool uses_decimal = p & d_mask;
    a = adc(a, b, uses_decimal);
    cycles(6 + uses_decimal);
}

void w65c02::Cpu65c02::adc_imm()
{
    bool uses_decimal = p & d_mask;
    a = adc(a, parser.imm0reg(), uses_decimal);
    cycles(2 + uses_decimal);
}

void w65c02::Cpu65c02::adc_sr()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::adc_sriy()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::and_abs()
{
    a &= mem.read8(parser.addr0reg());
    update_flags_n_z(a);
    cycles(4);
}

void w65c02::Cpu65c02::and_absl()
{
    bbr(2);
}

void w65c02::Cpu65c02::and_abslx()
{
    bbr(3);
}

void w65c02::Cpu65c02::and_absx()
{
    Address addr = parser.addr0reg() + x;
    a &= mem.read8(addr);
    update_flags_n_z(a);
    cycles(4 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::and_absy()
{
    Address addr = parser.addr0reg() + y;
    a &= mem.read8(addr);
    update_flags_n_z(a);
    cycles(4 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::and_dp()
{
    a &= mem.read8(parser.addr0reg());
    update_flags_n_z(a);
    cycles(3);
}

void w65c02::Cpu65c02::and_dpi()
{
    a &= mem.read8(Address(mem.read16(parser.addr0reg())));
    update_flags_n_z(a);
    cycles(5);
}

void w65c02::Cpu65c02::and_dpil()
{
    rmb(2);
}

void w65c02::Cpu65c02::and_dpily()
{
    rmb(3);
}

void w65c02::Cpu65c02::and_dpiy()
{
    Address addr = Address(mem.read16(parser.addr0reg()));
    Address addry = addr + y;
    a &= mem.read8(addry);
    update_flags_n_z(a);
    cycles(5 + pages_differ(addr, addry));
}

void w65c02::Cpu65c02::and_dpx()
{
    a &= mem.read8((parser.addr0reg() + x) & 0xff);
    update_flags_n_z(a);
    cycles(4);
}

void w65c02::Cpu65c02::and_dpxi()
{
    a &= mem.read8(Address(mem.read16((parser.addr0reg() + x) & 0xff)));
    update_flags_n_z(a);
    cycles(6);
}

void w65c02::Cpu65c02::and_imm()
{
    a &= parser.imm0reg();
    update_flags_n_z(a);
    cycles(2);
}

void w65c02::Cpu65c02::and_sr()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::and_sriy()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::asl_acc()
{
    a = asl(a);
    cycles(2);
}

void w65c02::Cpu65c02::asl_abs()
{
    asl(parser.addr0reg());
    cycles(6);
}

void w65c02::Cpu65c02::asl_absx()
{
    Address addr = parser.addr0reg() + x;
    asl(addr);
    cycles(6 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::asl_dp()
{
    asl(parser.addr0reg());
    cycles(5);
}

void w65c02::Cpu65c02::asl_dpx()
{
    asl(Address(parser.addr0reg() + x));
    cycles(6);
}

void w65c02::Cpu65c02::bcc()
{
    if (!(p & c_mask))
        branch_taken();
    cycles(2);
}

void w65c02::Cpu65c02::bcs()
{
    if (p & c_mask)
        branch_taken();
    cycles(2);
}

void w65c02::Cpu65c02::beq()
{
    if (p & z_mask)
        branch_taken();
    cycles(2);
}

void w65c02::Cpu65c02::bit_abs()
{
    bit(parser.addr0reg());
    cycles(4);
}

void w65c02::Cpu65c02::bit_absx()
{
    Address addr = parser.addr0reg() + x;
    bit(addr);
    cycles(4 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::bit_dp()
{
    bit(parser.addr0reg());
    cycles(3);
}

void w65c02::Cpu65c02::bit_dpx()
{
    bit(Address(parser.addr0reg() + x));
    cycles(4);
}

void w65c02::Cpu65c02::bit_imm()
{
    p &= ~z_mask;
    if (!(parser.imm0reg() & a))
        p |= z_mask;
    cycles(2);
}

void w65c02::Cpu65c02::bmi()
{
    if (p & n_mask)
        branch_taken();
    cycles(2);
}

void w65c02::Cpu65c02::bne()
{
    if (!(p & z_mask))
        branch_taken();
    cycles(2);
}

void w65c02::Cpu65c02::bpl()
{
    if (!(p & n_mask))
        branch_taken();
    cycles(2);
}

void w65c02::Cpu65c02::bra()
{
    Address regular_pc = parser.program_counter();
    parser.jmp(parser.addr0reg());
    cycles(3 + pages_differ(regular_pc, parser.program_counter()));
}

void w65c02::Cpu65c02::brk()
{
    push16v(parser.program_counter());
    pushv(p | b_mask);
    parser.jmp(Address(mem.read16(0xfffe)));
    cycles(7);
}

void w65c02::Cpu65c02::brl()
{
    unimplemented(2, 2);
}

void w65c02::Cpu65c02::bvc()
{
    if (!(p & v_mask))
        branch_taken();
    cycles(2);
}

void w65c02::Cpu65c02::bvs()
{
    if (p & v_mask)
        branch_taken();
    cycles(2);
}

void w65c02::Cpu65c02::clc()
{
    p &= ~c_mask;
    cycles(2);
}

void w65c02::Cpu65c02::cld()
{
    p &= ~d_mask;
    cycles(2);
}

void w65c02::Cpu65c02::cli()
{
    p &= ~i_mask;
    update_attention();
    cycles(2);
}

void w65c02::Cpu65c02::clv()
{
    p &= ~v_mask;
    cycles(2);
}

void w65c02::Cpu65c02::cmp_abs()
{
    cmp(a, mem.read8(parser.addr0reg()));
    cycles(4);
}

void w65c02::Cpu65c02::cmp_absl()
{
    bbs(4);
}

void w65c02::Cpu65c02::cmp_abslx()
{
    bbs(5);
}

void w65c02::Cpu65c02::cmp_absx()
{
    Address addr = parser.addr0reg() + x;
    cmp(a, mem.read8(addr));
    cycles(4 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::cmp_absy()
{
    Address addr = parser.addr0reg() + y;
    cmp(a, mem.read8(addr));
    cycles(4 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::cmp_dp()
{
    cmp(a, mem.read8(parser.addr0reg()));
    cycles(3);
}

void w65c02::Cpu65c02::cmp_dpi()
{
    cmp(a, mem.read8(Address(mem.read16(parser.addr0reg()))));
    cycles(5);
}

void w65c02::Cpu65c02::cmp_dpil()
{
    smb(4);
}

void w65c02::Cpu65c02::cmp_dpily()
{
    smb(5);
}

void w65c02::Cpu65c02::cmp_dpiy()
{
    Address addr = Address(mem.read16(parser.addr0reg()));
    Address addry = addr + y;
    cmp(a, mem.read8(addry));
    cycles(5 + pages_differ(addr, addry));
}

void w65c02::Cpu65c02::cmp_dpx()
{
    cmp(a, mem.read8((parser.addr0reg() + x) & 0xff));
    cycles(4);
}

void w65c02::Cpu65c02::cmp_dpxi()
{
    cmp(a, mem.read8(Address(mem.read16((parser.addr0reg() + x) & 0xff))));
    cycles(6);
}

void w65c02::Cpu65c02::cmp_imm()
{
    cmp(a, parser.imm0reg());
    cycles(2);
}

void w65c02::Cpu65c02::cmp_sr()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::cmp_sriy()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::cop()
{
    unimplemented(2, 2);
}

void w65c02::Cpu65c02::cpx_abs()
{
    cmp(x, mem.read8(parser.addr0reg()));
    cycles(4);
}

void w65c02::Cpu65c02::cpx_dp()
{
    cmp(x, mem.read8(parser.addr0reg()));
    cycles(3);
}

void w65c02::Cpu65c02::cpx_imm()
{
    cmp(x, parser.imm0reg());
    cycles(2);
}

void w65c02::Cpu65c02::cpy_abs()
{
    cmp(y, mem.read8(parser.addr0reg()));
    cycles(4);
}

void w65c02::Cpu65c02::cpy_dp()
{
    cmp(y, mem.read8(parser.addr0reg()));
    cycles(3);
}

void w65c02::Cpu65c02::cpy_imm()
{
    cmp(y, parser.imm0reg());
    cycles(2);
}

void w65c02::Cpu65c02::dec_acc()
{
    --a;
    update_flags_n_z(a);
    cycles(2);
}

void w65c02::Cpu65c02::dec_abs()
{
    dec(parser.addr0reg());
    cycles(6);
}

void w65c02::Cpu65c02::dec_absx()
{
    Address addr = parser.addr0reg() + x;
    dec(addr);
    cycles(6 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::dec_dp()
{
    dec(parser.addr0reg());
    cycles(5);
}

void w65c02::Cpu65c02::dec_dpx()
{
    dec(Address(parser.addr0reg() + x));
    cycles(6);
}

void w65c02::Cpu65c02::dex()
{
    --x;
    update_flags_n_z(x);
    cycles(2);
}

void w65c02::Cpu65c02::dey()
{
    --y;
    update_flags_n_z(y);
    cycles(2);
}

void w65c02::Cpu65c02::eor_abs()
{
    a ^= mem.read8(parser.addr0reg());
    update_flags_n_z(a);
    cycles(4);
}

void w65c02::Cpu65c02::eor_absl()
{
    bbr(4);
}

void w65c02::Cpu65c02::eor_abslx()
{
    bbr(5);
}

void w65c02::Cpu65c02::eor_absx()
{
    Address addr = parser.addr0reg() + x;
    a ^= mem.read8(addr);
    update_flags_n_z(a);
    cycles(4 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::eor_absy()
{
    Address addr = parser.addr0reg() + y;
    a ^= mem.read8(addr);
    update_flags_n_z(a);
    cycles(4 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::eor_dp()
{
    a ^= mem.read8(parser.addr0reg());
    update_flags_n_z(a);
    cycles(3);
}

void w65c02::Cpu65c02::eor_dpi()
{
    a ^= mem.read8(Address(mem.read16(parser.addr0reg())));
    update_flags_n_z(a);
    cycles(5);
}

void w65c02::Cpu65c02::eor_dpil()
{
    rmb(4);
}

void w65c02::Cpu65c02::eor_dpily()
{
    rmb(5);
}

void w65c02::Cpu65c02::eor_dpiy()
{
    Address addr = Address(mem.read16(parser.addr0reg()));
    Address addry = addr + y;
    a ^= mem.read8(addry);
    update_flags_n_z(a);
    cycles(5 + pages_differ(addr, addry));
}

void w65c02::Cpu65c02::eor_dpx()
{
    a ^= mem.read8((parser.addr0reg() + x) & 0xff);
    update_flags_n_z(a);
    cycles(4);
}

void w65c02::Cpu65c02::eor_dpxi()
{
    a ^= mem.read8(Address(mem.read16((parser.addr0reg() + x) & 0xff)));
    update_flags_n_z(a);
    cycles(6);
}

void w65c02::Cpu65c02::eor_imm()
{
    a ^= parser.imm0reg();
    update_flags_n_z(a);
    cycles(2);
}

void w65c02::Cpu65c02::eor_sr()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::eor_sriy()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::inc_acc()
{
    ++a;
    update_flags_n_z(a);
    cycles(2);
}

void w65c02::Cpu65c02::inc_abs()
{
    inc(parser.addr0reg());
    cycles(6);
}

void w65c02::Cpu65c02::inc_absx()
{
    Address addr = parser.addr0reg() + x;
    inc(addr);
    cycles(6 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::inc_dp()
{
    inc(parser.addr0reg());
    cycles(5);
}

void w65c02::Cpu65c02::inc_dpx()
{
    inc(Address(parser.addr0reg() + x));
    cycles(6);
}

void w65c02::Cpu65c02::inx()
{
    ++x;
    update_flags_n_z(x);
    cycles(2);
}

void w65c02::Cpu65c02::iny()
{
    ++y;
    update_flags_n_z(y);
    cycles(2);
}

void w65c02::Cpu65c02::jml()
{
    unimplemented(3, 8);
}

void w65c02::Cpu65c02::jmli()
{
    unimplemented(3, 4);
}

void w65c02::Cpu65c02::jmp_abs()
{
    parser.jmp(parser.addr0reg());
    cycles(3);
}

void w65c02::Cpu65c02::jmp_absi()
{
    parser.jmp(Address(mem.read16(parser.addr0reg())));
    cycles(6);
}

void w65c02::Cpu65c02::jmp_absxi()
{
    parser.jmp(Address(mem.read16(parser.addr0reg() + x)));
    cycles(6);
}

void w65c02::Cpu65c02::jsl()
{
    unimplemented(2, 2);
}

void w65c02::Cpu65c02::jsr_abs()
{
    push16v(parser.program_counter() - 1);
    parser.jmp(parser.addr0reg());
    cycles(6);
}

void w65c02::Cpu65c02::jsr_absxi()
{
    unimplemented(3, 4);
}

void w65c02::Cpu65c02::lda_abs()
{
    a = mem.read8(parser.addr0reg());
    update_flags_n_z(a);
    cycles(4);
}

void w65c02::Cpu65c02::lda_absl()
{
    bbs(2);
}

void w65c02::Cpu65c02::lda_abslx()
{
    bbs(3);
}

void w65c02::Cpu65c02::lda_absx()
{
    Address addr = parser.addr0reg() + x;
    a = mem.read8(addr);
    update_flags_n_z(a);
    cycles(4 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::lda_absy()
{
    Address addr = parser.addr0reg() + y;
    a = mem.read8(addr);
    update_flags_n_z(a);
    cycles(4 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::lda_dp()
{
    a = mem.read8(parser.addr0reg());
    update_flags_n_z(a);
    cycles(3);
}

void w65c02::Cpu65c02::lda_dpi()
{
    a = mem.read8(Address(mem.read16(parser.addr0reg())));
    update_flags_n_z(a);
    cycles(5);
}

void w65c02::Cpu65c02::lda_dpil()
{
    smb(2);
}

void w65c02::Cpu65c02::lda_dpily()
{
    smb(3);
}

void w65c02::Cpu65c02::lda_dpiy()
{
    Address addr = Address(mem.read16(parser.addr0reg()));
    Address addry = addr + y;
    a = mem.read8(addry);
    update_flags_n_z(a);
    cycles(5 + pages_differ(addr, addry));
}

void w65c02::Cpu65c02::lda_dpx()
{
    a = mem.read8((parser.addr0reg() + x) & 0xff);
    update_flags_n_z(a);
    cycles(4);
}

void w65c02::Cpu65c02::lda_dpxi()
{
    a = mem.read8(Address(mem.read16((parser.addr0reg() + x) & 0xff)));
    update_flags_n_z(a);
    cycles(6);
}

void w65c02::Cpu65c02::lda_imm()
{
    a = parser.imm0reg();
    update_flags_n_z(a);
    cycles(2);
}

void w65c02::Cpu65c02::lda_sr()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::lda_sriy()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::ldx_abs()
{
    x = mem.read8(parser.addr0reg());
    update_flags_n_z(x);
    cycles(4);
}

void w65c02::Cpu65c02::ldx_absy()
{
    Address addr = parser.addr0reg() + y;
    x = mem.read8(addr);
    update_flags_n_z(x);
    cycles(4 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::ldx_dp()
{
    x = mem.read8(parser.addr0reg());
    update_flags_n_z(x);
    cycles(3);
}

void w65c02::Cpu65c02::ldx_dpy()
{
    x = mem.read8((parser.addr0reg() + y) & 0xff);
    update_flags_n_z(x);
    cycles(4);
}

void w65c02::Cpu65c02::ldx_imm()
{
    x = parser.imm0reg();
    update_flags_n_z(x);
    cycles(2);
}

void w65c02::Cpu65c02::ldy_abs()
{
    y = mem.read8(parser.addr0reg());
    update_flags_n_z(a);
    cycles(4);
}

void w65c02::Cpu65c02::ldy_absx()
{
    Address addr = parser.addr0reg() + x;
    y = mem.read8(addr);
    update_flags_n_z(y);
    cycles(4 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::ldy_dp()
{
    y = mem.read8(parser.addr0reg());
    update_flags_n_z(y);
    cycles(3);
}

void w65c02::Cpu65c02::ldy_dpx()
{
    y = mem.read8((parser.addr0reg() + x) & 0xff);
    update_flags_n_z(y);
    cycles(4);
}

void w65c02::Cpu65c02::ldy_imm()
{
    y = parser.imm0reg();
    update_flags_n_z(y);
    cycles(2);
}

void w65c02::Cpu65c02::lsr_acc()
{
    a = lsr(a);
    cycles(2);
}

void w65c02::Cpu65c02::lsr_abs()
{
    lsr(parser.addr0reg());
    cycles(6);
}

void w65c02::Cpu65c02::lsr_absx()
{
    Address addr = parser.addr0reg() + x;
    lsr(addr);
    cycles(6 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::lsr_dp()
{
    lsr(parser.addr0reg());
    cycles(5);
}

void w65c02::Cpu65c02::lsr_dpx()
{
    lsr(Address(parser.addr0reg() + x));
    cycles(6);
}

void w65c02::Cpu65c02::mvn()
{
    unimplemented(2, 4);
}

void w65c02::Cpu65c02::mvp()
{
    unimplemented(2, 3);
}

void w65c02::Cpu65c02::nop()
{
    cycles(2);
}

void w65c02::Cpu65c02::ora_abs()
{
    a |= mem.read8(parser.addr0reg());
    update_flags_n_z(a);
    cycles(4);
}

void w65c02::Cpu65c02::ora_absl()
{
    bbr(0);
}

void w65c02::Cpu65c02::ora_abslx()
{
    bbr(1);
}

void w65c02::Cpu65c02::ora_absx()
{
    Address addr = parser.addr0reg() + x;
    a |= mem.read8(addr);
    update_flags_n_z(a);
    cycles(4 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::ora_absy()
{
    Address addr = parser.addr0reg() + y;
    a |= mem.read8(addr);
    update_flags_n_z(a);
    cycles(4 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::ora_dp()
{
    a |= mem.read8(parser.addr0reg());
    update_flags_n_z(a);
    cycles(3);
}

void w65c02::Cpu65c02::ora_dpi()
{
    a |= mem.read8(Address(mem.read16(parser.addr0reg())));
    update_flags_n_z(a);
    cycles(5);
}

void w65c02::Cpu65c02::ora_dpil()
{
    rmb(0);
}

void w65c02::Cpu65c02::ora_dpily()
{
    rmb(1);
}

void w65c02::Cpu65c02::ora_dpiy()
{
    Address addr = Address(mem.read16(parser.addr0reg()));
    Address addry = addr + y;
    a |= mem.read8(addry);
    update_flags_n_z(a);
    cycles(5 + pages_differ(addr, addry));
}

void w65c02::Cpu65c02::ora_dpx()
{
    a |= mem.read8((parser.addr0reg() + x) & 0xff);
    update_flags_n_z(a);
    cycles(4);
}

void w65c02::Cpu65c02::ora_dpxi()
{
    a |= mem.read8(Address(mem.read16((parser.addr0reg() + x) & 0xff)));
    update_flags_n_z(a);
    cycles(6);
}

void w65c02::Cpu65c02::ora_imm()
{
    a |= parser.imm0reg();
    update_flags_n_z(a);
    cycles(2);
}

void w65c02::Cpu65c02::ora_sr()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::ora_sriy()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::pea()
{
    unimplemented(2, 4);
}

void w65c02::Cpu65c02::pei()
{
    unimplemented(2, 4);
}

void w65c02::Cpu65c02::per()
{
    unimplemented(2, 2);
}

void w65c02::Cpu65c02::pha()
{
    pushv(a);
    cycles(3);
}

void w65c02::Cpu65c02::phb()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::phd()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::phk()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::php()
{
    pushv(p);
    cycles(3);
}

void w65c02::Cpu65c02::phx()
{
    pushv(x);
    cycles(3);
}

void w65c02::Cpu65c02::phy()
{
    pushv(y);
    cycles(3);
}

void w65c02::Cpu65c02::pla()
{
    a = pullv();
    update_flags_n_z(a);
    cycles(4);
}

void w65c02::Cpu65c02::plb()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::pld()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::plp()
{
    p &= 0x30;
    p |= pullv();
    cycles(4);
}

void w65c02::Cpu65c02::plx()
{
    x = pullv();
    update_flags_n_z(x);
    cycles(4);
}

void w65c02::Cpu65c02::ply()
{
    y = pullv();
    update_flags_n_z(y);
    cycles(4);
}

void w65c02::Cpu65c02::rep()
{
    unimplemented(2, 2);
}

void w65c02::Cpu65c02::rol_acc()
{
    a = rol(a);
    cycles(2);
}

void w65c02::Cpu65c02::rol_abs()
{
    rol(parser.addr0reg());
    cycles(6);
}

void w65c02::Cpu65c02::rol_absx()
{
    Address addr = parser.addr0reg() + x;
    rol(addr);
    cycles(6 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::rol_dp()
{
    rol(parser.addr0reg());
    cycles(5);
}

void w65c02::Cpu65c02::rol_dpx()
{
    rol(Address(parser.addr0reg() + x));
    cycles(6);
}

void w65c02::Cpu65c02::ror_acc()
{
    a = ror(a);
    cycles(2);
}

void w65c02::Cpu65c02::ror_abs()
{
    ror(parser.addr0reg());
    cycles(6);
}

void w65c02::Cpu65c02::ror_absx()
{
    Address addr = parser.addr0reg() + x;
    ror(addr);
    cycles(6 + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::ror_dp()
{
    ror(parser.addr0reg());
    cycles(5);
}

void w65c02::Cpu65c02::ror_dpx()
{
    ror(Address(parser.addr0reg() + x));
    cycles(6);
}

void w65c02::Cpu65c02::rti()
{
    p = pullv() | u_mask | b_mask;
    parser.jmp(Address(pull16v()));
    cycles(6);
}

void w65c02::Cpu65c02::rtl()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::rts()
{
    parser.jmp(Address(pull16v()) + 1);
    cycles(6);
}

void w65c02::Cpu65c02::sbc_abs()
{
    Byte b = mem.read8(parser.addr0reg());
    bool uses_decimal = p & d_mask;
    a = sbc(a, b, uses_decimal);
    cycles(4 + uses_decimal);
}

void w65c02::Cpu65c02::sbc_absl()
{
    bbs(6);
}

void w65c02::Cpu65c02::sbc_abslx()
{
    bbs(7);
}

void w65c02::Cpu65c02::sbc_absx()
{
    Address addr = parser.addr0reg() + x;
    Byte b = mem.read8(addr);
    bool uses_decimal = p & d_mask;
    a = sbc(a, b, uses_decimal);
    cycles(4 + uses_decimal + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::sbc_absy()
{
    Address addr = parser.addr0reg() + y;
    Byte b = mem.read8(addr);
    bool uses_decimal = p & d_mask;
    a = sbc(a, b, uses_decimal);
    cycles(4 + uses_decimal + pages_differ(parser.addr0reg(), addr));
}

void w65c02::Cpu65c02::sbc_dp()
{
    Byte b = mem.read8(parser.addr0reg());
    bool uses_decimal = p & d_mask;
    a = sbc(a, b, uses_decimal);
    cycles(3 + uses_decimal);
}

void w65c02::Cpu65c02::sbc_dpi()
{
    Byte b = mem.read8(Address(mem.read16(parser.addr0reg())));
    bool uses_decimal = p & d_mask;
    a = sbc(a, b, uses_decimal);
    cycles(5 + uses_decimal);
}

void w65c02::Cpu65c02::sbc_dpil()
{
    smb(6);
}

void w65c02::Cpu65c02::sbc_dpily()
{
    smb(7);
}

void w65c02::Cpu65c02::sbc_dpiy()
{
    Address addr = Address(mem.read16(parser.addr0reg()));
    Address addry = addr + y;
    Byte b = mem.read8(addry);
    bool uses_decimal = p & d_mask;
    a = sbc(a, b, uses_decimal);
    cycles(5 + uses_decimal + pages_differ(addr, addry));
}

void w65c02::Cpu65c02::sbc_dpx()
{
    Byte b = mem.read8((parser.addr0reg() + x) & 0xff);
    bool uses_decimal = p & d_mask;
    a = sbc(a, b, uses_decimal);
    cycles(4 + uses_decimal);
}

void w65c02::Cpu65c02::sbc_dpxi()
{
    Byte b = mem.read8(Address(mem.read16((parser.addr0reg() + x) & 0xff)));
    bool uses_decimal = p & d_mask;
    a = sbc(a, b, uses_decimal);
    cycles(6 + uses_decimal);
}

void w65c02::Cpu65c02::sbc_imm()
{
    bool uses_decimal = p & d_mask;
    a = sbc(a, parser.imm0reg(), uses_decimal);
    cycles(2 + uses_decimal);
}

void w65c02::Cpu65c02::sbc_sr()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::sbc_sriy()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::sec()
{
    p |= c_mask;
    cycles(2);
}

void w65c02::Cpu65c02::sed()
{
    p |= d_mask;
    cycles(2);
}

void w65c02::Cpu65c02::sei()
{
    p |= i_mask;
    update_attention();
    cycles(2);
}

void w65c02::Cpu65c02::sep()
{
    unimplemented(2, 2);
}

void w65c02::Cpu65c02::sta_abs()
{
    mem.write8(parser.addr0reg(), a);
    cycles(4);
}

void w65c02::Cpu65c02::sta_absl()
{
    bbs(0);
}

void w65c02::Cpu65c02::sta_abslx()
{
    bbs(1);
}

void w65c02::Cpu65c02::sta_absx()
{
    mem.write8(parser.addr0reg() + x, a);
    cycles(5);
}

void w65c02::Cpu65c02::sta_absy()
{
    mem.write8(parser.addr0reg() + y, a);
    cycles(5);
}

void w65c02::Cpu65c02::sta_dp()
{
    mem.write8(parser.addr0reg(), a);
    cycles(3);
}

void w65c02::Cpu65c02::sta_dpi()
{
    mem.write8(Address(mem.read16(parser.addr0reg())), a);
    cycles(5);
}

void w65c02::Cpu65c02::sta_dpil()
{
    smb(0);
}

void w65c02::Cpu65c02::sta_dpily()
{
    smb(1);
}

void w65c02::Cpu65c02::sta_dpiy()
{
    Address addr = Address(mem.read16(parser.addr0reg()));
    Address addry = addr + y;
    mem.write8(addry, a);
    cycles(6);
}

void w65c02::Cpu65c02::sta_dpx()
{
    mem.write8((parser.addr0reg() + x) & 0xff, a);
    cycles(4);
}

void w65c02::Cpu65c02::sta_dpxi()
{
    mem.write8(Address(mem.read16((parser.addr0reg() + x) & 0xff)), a);
    cycles(6);
}

void w65c02::Cpu65c02::sta_sr()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::sta_sriy()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::stp()
{
    if (pausable)
    {
        cycles(3);
        while (vdd && !resb_sequence)
            this_thread::sleep_for(wait_period());
    }
    else
        unimplemented(1, 1);
}

void w65c02::Cpu65c02::stx_abs()
{
    mem.write8(parser.addr0reg(), x);
    cycles(4);
}

void w65c02::Cpu65c02::stx_dp()
{
    mem.write8(parser.addr0reg(), x);
    cycles(3);
}

void w65c02::Cpu65c02::stx_dpy()
{
    mem.write8((parser.addr0reg() + y) & 0xff, x);
    cycles(4);
}

void w65c02::Cpu65c02::sty_abs()
{
    mem.write8(parser.addr0reg(), y);
    cycles(4);
}

void w65c02::Cpu65c02::sty_dp()
{
    mem.write8(parser.addr0reg(), y);
    cycles(3);
}

void w65c02::Cpu65c02::sty_dpx()
{
    mem.write8((parser.addr0reg() + x) & 0xff, y);
    cycles(4);
}

void w65c02::Cpu65c02::stz_abs()
{
    mem.write8(parser.addr0reg(), 0);
    cycles(4);
}

void w65c02::Cpu65c02::stz_absx()
{
    mem.write8(parser.addr0reg() + x, 0);
    cycles(5);
}

void w65c02::Cpu65c02::stz_dp()
{
    mem.write8(parser.addr0reg(), 0);
    cycles(3);
}

void w65c02::Cpu65c02::stz_dpx()
{
    mem.write8((parser.addr0reg() + x) & 0xff, 0);
    cycles(4);
}

void w65c02::Cpu65c02::tax()
{
    x = a;
    update_flags_n_z(x);
    cycles(2);
}

void w65c02::Cpu65c02::tay()
{
    y = a;
    update_flags_n_z(y);
    cycles(2);
}

void w65c02::Cpu65c02::tcd()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::tcs()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::tdc()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::trb_abs()
{
    trb(parser.addr0reg());
    cycles(6);
}

void w65c02::Cpu65c02::trb_dp()
{
    trb(parser.addr0reg());
    cycles(5);
}

void w65c02::Cpu65c02::tsb_abs()
{
    tsb(parser.addr0reg());
    cycles(6);
}

void w65c02::Cpu65c02::tsb_dp()
{
    tsb(parser.addr0reg());
    cycles(5);
}

void w65c02::Cpu65c02::tsc()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::tsx()
{
    x = s;
    update_flags_n_z(x);
    cycles(2);
}

void w65c02::Cpu65c02::txa()
{
    a = x;
    update_flags_n_z(a);
    cycles(2);
}

void w65c02::Cpu65c02::txs()
{
    s = x;
    cycles(2);
}

void w65c02::Cpu65c02::txy()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::tya()
{
    a = y;
    update_flags_n_z(a);
    cycles(2);
}

void w65c02::Cpu65c02::tyx()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::wai()
{
    if (pausable)
    {
        cycles(3);
        bool interrupted = resb_sequence || nmi_sequence || irqline_low;
        while (vdd && ! interrupted)
        {
            this_thread::sleep_for(wait_period());
            interrupted = resb_sequence || nmi_sequence || irqline_low;
        }
    }
    else
        unimplemented(1, 1);
}

void w65c02::Cpu65c02::wdm()
{
    unimplemented(2, 2);
}

void w65c02::Cpu65c02::xba()
{
    unimplemented(1, 1);
}

void w65c02::Cpu65c02::xce()
{
    unimplemented(1, 1);
}
