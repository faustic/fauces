// Test machine
//
// Created by Alejandro Castro García on 3 May 2023
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


#include "machine.hpp"
#include "executable.hpp"
#include "assembler.hpp"
#include "disassembler.hpp"

#include <faulib/io/rawfile.hpp>

#include <sstream>
#include <iostream>
#include <iomanip>

using std::cout;
using std::hex;
using std::setw;
using std::setfill;

void w65c02::Mem_test::action(Address addr, Byte v)
{
    switch (addr)
    {
        case poweroff:
            machine.power(false);
            return;
            
        case report_brk:
        {
            Byte s = machine.cpu().sreg();
            Word p = read8(0x100 + ((s + 1) & 0xff));
            Address pc = read16(0x100 + ((s + 2) & 0xff));
            cout << "BRK\n";
            cout << hex << setfill('0');
            cout << "P: $" << setw(2) << p << '\n';
            cout << "PC: $" << setw(4) << pc << '\n';
            break;
        }
        case report_res:
            cout << "RES\n";
            break;
        case report_irq:
            cout << "IRQ\n";
            break;
        case report_nmi:
            cout << "NMI\n";
            break;
        case interrupt_done:
            cout << "IRQ handled\n";
            machine.irq_clear();
            break;
            
    }
    Mem_le_plain::write8(addr, v);
}

void w65c02::Test_machine::turn_on()
{
    Executable exec(mem);
    exec.load(test_path + "/TEST");
    result_address = exec.result_start();
    result_size = exec.result_size();
    
    Address start {0xf000};
    Assembler as {mem, start};
    
    as.stz(Abs(Mem_test::report_res));
    as.bra("poweroff");
    mem.write16(0xfffc, as.program_counter()); // Initial reset vector
    as.lda(Imm(start & 0xff)); // Runtime reset vector
    as.sta(Abs(0xfffc));
    as.lda(Imm(start >> 8));
    as.sta(Abs(0xfffd));
    as.cli();
    as.jsr(Abs(exec.start())); // Jump to test code

    as.label("poweroff");
    as.stz(Abs(Mem_test::poweroff));
    as.label("wait_poweroff");
    as.bra("wait_poweroff");
    
    mem.write16(0xfffe, as.program_counter()); // Break/IRQ vector
    as.pla();
    as.pha();
    as.And(Imm(1 << 4));
    as.bne("brk_handler");
    as.stz(Abs(Mem_test::report_irq));
    as.stz(Abs(Mem_test::interrupt_done));
    as.rti();
    as.label("brk_handler");
    as.stz(Abs(Mem_test::report_brk));
    as.bra("poweroff");
    
    mem.write16(0xfffa, as.program_counter()); // NMI vector
    as.stz(Abs(Mem_test::report_nmi));
    as.bra("poweroff");

    /*
    Disassembler disas(mem, 0x2020, 600, true);
    cout << disas;
    */
    
    power(true);
    cpu().reset_low();
    cpu().turn_on();
    cpu().reset_high();
    ui.start_events();
    while (on)
    {
        using namespace std::chrono_literals;
        this_thread::sleep_for(1ms);
        ui.next_event();
    }
    ui.end_events();
    off();
}

void w65c02::Test_machine::off()
{
    cpu().turn_off();
    compare();
}

void w65c02::Test_machine::compare()
{
    using faulib::io::readraw;
    if (result_size > 0)
    {
        string result_path = test_path + "/RESULT";
        auto bytes = readraw<Byte>(result_path.c_str());
        if (bytes.size() != result_size)
            throw Test_machine_error("Bad result size");
        for (Word i = 0; i != result_size; ++i)
        {
            Address addr = result_address + i;
            Byte value = mem.read8(addr);
            if (bytes[i] != value)
            {
                std::stringstream stream;
                stream << "Results differ at $" << hex << setfill('0');
                stream << setw(4) << (unsigned)i;
                stream << " ($" << setw(4) << (unsigned)addr << ")";
                stream << "\nExpected: $" << setw(2) << (unsigned)bytes[i];
                stream << "\nObtained: $" << setw(2) << (unsigned)value;
                string msg = stream.str();
                throw Test_machine_error(msg);
            }
        }
    }
    cout << "Test OK\n\n";
}

