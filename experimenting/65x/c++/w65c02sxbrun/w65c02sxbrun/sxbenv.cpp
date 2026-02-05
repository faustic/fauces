// sxbenv.cpp
// Minimal environment where 65C02 test programs will run: implementation
//
// Created by Alejandro Castro García on 3 September 2021
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


#include "sxbenv.hpp"

auto w65c02::send_call(Sxb& sxb, Address pc, Address called_addr) -> Address
{
    constexpr unsigned long size = 3;
    vector<Byte> addr_bytes = called_addr.get_bytes();
    array<Byte, size> code
    {
        0x20, addr_bytes[0], addr_bytes[1] // JSR addr
    };
    sxb.from_pc(pc, code);
    return pc + size;
}

auto w65c02::send_jump(Sxb& sxb, Address pc, Address called_addr) -> Address
{
    constexpr unsigned long size = 3;
    vector<Byte> addr_bytes = called_addr.get_bytes();
    array<Byte, size> code
    {
        0x4c, addr_bytes[0], addr_bytes[1] // JMP addr
    };
    sxb.from_pc(pc, code);
    return pc + size;
}

auto w65c02::send_break(Sxb& sxb, Address pc, Byte arg) -> Address
{
    constexpr unsigned long size = 2;
    array<Byte, size> code
    {
        0x00, arg // BRK arg
    };
    sxb.from_pc(pc, code);
    return pc + size;
}

auto w65c02::send_led_routine(Sxb &sxb, Address pc, Xcs chip) -> Address
{
    constexpr unsigned long size = 18;
    auto xcs = Address(0x7f00 + 0x20 * static_cast<Byte>(chip)).get_bytes();
    array<Byte, size> code
    {
        0xda,                           // PHX
        0x5a,                           // PHY
        0xa0, 0x00,                     // LDY #$00
//EL:
        0xa2, 0x00,                     // LDX #$00
//IL:
        0xca,                           // DEX
        0x9c, xcs.at(0), xcs.at(1),     // STZ XCS
        0xd0, 0xfa,                     // BNE IL
        0x88,                           // DEY
        0xd0, 0xf5,                     // BNE EL
        0x7a,                           // PLY
        0xfa,                           // PLX
        0x60                            // RTS
    };
    sxb.from_pc(pc, code);
    return pc + size;
}

auto w65c02::send_break_after_repeated_call
    (Sxb& sxb, Address pc, Xcs, unsigned count, Address called_addr) -> Address
{
    for (unsigned i = 0; i < count; ++i)
        pc = send_call(sxb, pc, called_addr);
    pc = send_break(sxb, pc, 0);
    return pc;
}

static constexpr unsigned vectoraddrs[] = {
    0x7e72,     // NMI (e == 1)
    0x7e70,     // IRQ/BRK  (e == 1)
    0x7ef8,     // ABORT (e == 1)
    0x7ef4,     // COP (e == 1)
    0x7eee,     // IRQ (e == 0)
    0x7e76,     // NMI (e == 0)
    0x7ee8,     // ABORT (e == 0)
    0x7e74,     // BRK (e == 0)
    0x7ee4      // COP (e == 0)
};

auto w65c02::get_vectors(Sxb& sxb) -> vector<Address16>
{
    // Except for the RES interrupt, ROM interrupt routines simply jump to
    // addresses saved at certain RAM addresses. The get_vectors function
    // reads the current value of these RAM interrupt vectors.

    vector<Address16> vectors;
    if (sxb.is_65c02() || sxb.is_65c816())
    {
        auto nmi = sxb.to_pc(vectoraddrs[0], 2);
        auto irq = sxb.to_pc(vectoraddrs[1], 2);
        vectors.push_back(static_cast<Address16>(nmi));
        vectors.push_back(static_cast<Address16>(irq));
        if (sxb.is_65c816())
        {
            size_t l = sizeof(vectoraddrs) / sizeof(unsigned);
            for (size_t i = 2; i < l; ++i)
            {
                auto v = sxb.to_pc(vectoraddrs[i], 2);
                vectors.push_back(static_cast<Address16>(v));
            }
        }
    }
    return vectors;
}

void w65c02::send_vectors(Sxb& sxb, vector<Address16> vectors)
{
    try
    {
        if (sxb.is_65c02() || sxb.is_65c816())
        {
            auto bytes = vectors.at(0).get_bytes();
            sxb.from_pc(vectoraddrs[0], bytes); // NMI
            bytes = vectors.at(1).get_bytes();
            sxb.from_pc(vectoraddrs[1], bytes); // IRQ/BRK
            if (sxb.is_65c816())
            {
                size_t l = sizeof(vectoraddrs) / sizeof(unsigned);
                for (size_t i = 2; i < l; ++i)
                {
                    bytes = vectors.at(i).get_bytes();
                    sxb.from_pc(vectoraddrs[i], bytes);
                }
            }
        }
    }
    catch (std::out_of_range)
    {
        // I accept that not all vectors are necessarily being set, so I simply
        // return if there are fewer values in vectors than addresses in
        // vectoraddrs. However, the missing vectors should correspond to the
        // addresses that are last in vectoraddrs.
        return;
    }
}

auto w65c02::send_infinite_loop(Sxb& sxb, Address pc) -> Address
{
    constexpr unsigned long size = 2;
    array<Byte, size> code
    {
//L:
        0x80, 0xfe                      // BRA L
    };
    sxb.from_pc(pc, code);
    return pc + size;
}

auto w65c02::send_rti(Sxb& sxb, Address pc) -> Address
{
    constexpr unsigned long size = 1;
    array<Byte, size> code
    {
        0x40                      // RTI
    };
    sxb.from_pc(pc, code);
    return pc + size;
}

static w65c02::vector<w65c02::Address16> old_vectors;

auto w65c02::irq_brk_routine(Sxb& sxb, Address& pc) -> Address16
{
    Address irq_led_routine = pc;
    pc = send_led_routine(sxb, pc, Xcs::three);
    Address16 irq_routine = static_cast<Address16>(pc);
    pc = send_call(sxb, pc, irq_led_routine);
    pc = send_rti(sxb, pc);
    Address brk_led_routine = pc;
    pc = send_led_routine(sxb, pc, Xcs::two);
    Address16 brk_routine = static_cast<Address16>(pc);
    pc = send_call(sxb, pc, brk_led_routine);
    pc = send_jump(sxb, pc, old_vectors[1]);
    Address16 savea = static_cast<Address16>(pc);
    pc = pc + 1;
    static constexpr unsigned brk_mask = 0b0001'0000;
    static constexpr unsigned long code_size = 21;
    auto brk_bytes = brk_routine.get_bytes();
    auto irq_bytes = irq_routine.get_bytes();
    Byte savea_low {static_cast<Byte>(savea & 0xff)};
    Byte savea_high {static_cast<Byte>(savea >> 8)};
    Address16 irq_brk_routine = static_cast<Address16>(pc);
    array<Byte, code_size> code
    {
        0x8d, savea_low, savea_high,            // STA savea
        0x68,                                   // PLA
        0x48,                                   // PHA
        0x29, brk_mask,                         // AND  brk_mask
        0xf0, 0x06,                             // BEQ IRQ
        0xad, savea_low, savea_high,            // LDA savea
        0x4c, brk_bytes.at(0), brk_bytes.at(1), // JMP brk_routine
//IRQ:
        0xad, savea_low, savea_high,            // LDA savea
        0x4c, irq_bytes.at(0), irq_bytes.at(1)  // JMP irq_routine
    };
    sxb.from_pc(pc, code);
    pc = pc + code_size;
    return irq_brk_routine;
}

auto w65c02::interrupt_routines(Sxb& sxb, Address pc) -> Address
{
    old_vectors = get_vectors(sxb);
    
    Address led_routine = pc;
    pc = send_led_routine(sxb, pc, Xcs::one);
    Address16 nmi_routine = static_cast<Address16>(pc);
    pc = send_call(sxb, pc, led_routine);
    // pc = send_break(sxb, pc, 0);
    pc = send_rti(sxb, pc);
    // pc = send_jump(sxb, pc, old_vectors[0]);
    
    Address16 irq_routine = irq_brk_routine(sxb, pc);

    vector<Address16> vectors {nmi_routine, irq_routine};
    send_vectors(sxb, vectors);
    return pc;
}

auto w65c02::bridge_led(Sxb& sxb, Address prog_addr) -> Address
{
    Address bridge_addr = 0x1000;
    Address pc = bridge_addr;
    pc = send_call(sxb, pc, prog_addr);
    unsigned times = 5;
    Address led_routine = pc + 2 + 3 * times;
    pc = send_break_after_repeated_call(sxb, pc, Xcs::one, times, led_routine);
    if (led_routine != pc)
        throw Error_sxbenv_code_misplaced();
    send_led_routine(sxb, led_routine, Xcs::one);
    return bridge_addr;
}

auto w65c02::bridge_infinite(Sxb& sxb, Address prog_addr) -> Address
{
    Address bridge_addr = 0x1000;
    Address pc = bridge_addr;
    pc = send_call(sxb, pc, prog_addr);
    pc = send_infinite_loop(sxb, pc);
    pc = interrupt_routines(sxb, pc);
    return bridge_addr;
}

auto w65c02::bridge_interrupts(Sxb& sxb, Address prog_addr) -> Address
{
    Address bridge_addr = 0x1000;
    Address pc = bridge_addr;
    pc = send_call(sxb, pc, prog_addr);
    pc = send_break(sxb, pc, 0);
    pc = interrupt_routines(sxb, pc);
    return bridge_addr;
}

auto w65c02::bridge_plain(Sxb& sxb, Address prog_addr) -> Address
{
    Address bridge_addr = 0x1000;
    Address pc = bridge_addr;
    pc = send_call(sxb, pc, prog_addr);
    send_break(sxb, pc, 0);
    return bridge_addr;
}

