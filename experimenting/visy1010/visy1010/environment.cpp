// environment.cpp
// Virtual system environment
// Intended compatibility: c++17
//
// Created by Alejandro Castro Garcia on 22 December 2020
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


#include "environment.hpp"
#include "disassembler.hpp"
#include "assembler.hpp"
#include "exec.hpp"

#include "using_iostream.hpp"
#include "using_string.hpp"
#include "using_cstdint.hpp"

namespace vs
{

Environment::Environment(unsigned bits) : cpu(bits)
{
    /*
    Assembler as {cpu.code_ram(), cpu.data_ram()};
    as.lihz(0, R(0));
    as.lihz(0, R(1));
    as.sys(R(1), R(0));
    as.end();
     */
/*    Assembler as {cpu.code_ram(), cpu.data_ram()};
    string main_sym = "main";
    as.call(main_sym, R(1));
    as.lihz(0, R(1));
    as.sys(R(1), R(0));
    as.label(main_sym);
    as.xorb(R(0), R(0));
    as.ret();
    as.end();
    Disassembler disas{cpu.code_ram(), 0, 20};
    cout << static_cast<string>(disas);
    abort();*/
}

void Environment::load_exec(int argc, char **argv)
{
    if (argc > 0)
    {
        cout << "argv[0]: " << argv[0] << "\n";
        Program prog {argv[0]};
        cpu.code_ram().load(prog.code(), 0);
        cpu.sdata_ram().load(prog.data(), 0);
        Disassembler disas{cpu.code_ram(), 0, 20};
        cout << static_cast<string>(disas);
    }
    else
        throw No_program();
}

void Environment::start(int argc, char** argv)
{
    cout << "argc: " << argc << '\n';
    load_exec(argc, argv);
    on = true;
    cpu.reset();
    while (on)
    {
        try
        {
            cpu.loop();
        }
        catch(Unimplemented_instruction_error e)
        {
            on = false;
            cout << setfill('0');
            cout << "Unimplemented instruction 0x" << hex << setw(2) <<
                        (unsigned)e.instruction << "\n";
            cout << static_cast<string>(
                                Disassembler {cpu.code_ram(), e.position, 1});
        }
        catch(System_trap e)
        {
           system_trap(e.s, e.d);
        }
    }
}

Environment::Trap_handler Environment::handlers[handler_max + 1] =
{
    &Environment::leave,
    &Environment::unsupported,
    &Environment::unsupported,
    &Environment::unsupported,
    &Environment::unsupported,
    &Environment::unsupported,
    &Environment::unsupported,
    &Environment::unsupported,
    &Environment::unsupported,
    &Environment::unsupported,
    &Environment::unsupported,
    &Environment::unsupported,
    &Environment::unsupported,
    &Environment::unsupported,
    &Environment::unsupported,
    &Environment::unsupported
};

void Environment::system_trap(R s, R d)
{
    if (s == 1 && d == 0)
    {
        uint_least64_t constexpr mask = handler_max;
        uint_least64_t constexpr cmask = ~mask;
        uint_least64_t code = cpu.r1();
        uint_least64_t param = cpu.r0();
        if (code & cmask)
            throw Unsupported_trap(code, param);
        (this->*handlers[code & mask])(code, param);

    }
    else
        throw Unsupported_format();
}

void Environment::unsupported(uint_least64_t code, uint_least64_t param)
{
    on = false;
    throw Unsupported_trap(code, param);
}

void Environment::leave(uint_least64_t code, uint_least64_t param)
{
    on = false;
    leave_code = param & 0xffff;
}

} // namespace vs
