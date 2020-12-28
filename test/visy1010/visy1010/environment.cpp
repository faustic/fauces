// environment.cpp
// ** Placeholder to state the purpose of this file in very few words **
// Intended compatibility: c++17
//
// Created by Alejandro Castro Garcia on 22 December 2020
/*
Licensed under the MIT License.
 
Copyright (c) 2020 Faustic Inferno SL
 
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

#include "using_iostream.hpp"
#include "using_string.hpp"
#include "using_cstdint.hpp"

namespace vs
{

template <typename reg_type1, typename reg_type2>
Memory::byte_type instruction_code(Memory::byte_type opcode, reg_type1 s,
                                                                    reg_type2 d)
{
    static_assert(std::is_base_of<Register, reg_type1>::value);
    static_assert(std::is_base_of<Register, reg_type2>::value);
    Memory::byte_type code = static_cast<Memory::byte_type>(opcode << 2);
    code |= s << 1;
    code |= d;
    return code;
}

class Assembler
{
public:
    Assembler(Memory& cram, Bus64& dram) : cram {cram}, dram {dram}
    {
    }
    
/********** Instructions **********/
    void andb(R s, R d)
    {
        cram.at(pc++) = instruction_code(0x30, s, d);
    }
    
    void call(R d)
    {
        cram.at(pc++) = instruction_code(0x02, R(0), d);
    }
    
    void jmp(R d)
    {
        cram.at(pc++) = instruction_code(0x01, R(0), d);
    }
    
    void jmpnz(R s, R d)
    {
        cram.at(pc++) = instruction_code(0x05, R(s), d);
    }
    
    void jmpz(R s, R d)
    {
        cram.at(pc++) = instruction_code(0x04, R(s), d);
    }
    
    void least(R s, R d)
    {
        cram.at(pc++) = instruction_code(0x34, s, d);
    }
    
    void lmb(R s, R d)
    {
        cram.at(pc++) = instruction_code(0x10, s, d);
    }
    
    void lmd(R s, R d)
    {
        cram.at(pc++) = instruction_code(0x13, s, d);
    }
    
    void lmh(R s, R d)
    {
        cram.at(pc++) = instruction_code(0x11, s, d);
    }
    
    void lmw(R s, R d)
    {
        cram.at(pc++) = instruction_code(0x12, s, d);
    }
    
    void lrr(R s, R d)
    {
        cram.at(pc++) = instruction_code(0x1a, s, d);
    }
    
    void lrs(R s, S d)
    {
        cram.at(pc++) = instruction_code(0x1b, s, d);
    }
    
    void lrx(R s, X d)
    {
        cram.at(pc++) = instruction_code(0x18, s, d);
    }
    
    void lsr(S s, R d)
    {
        cram.at(pc++) = instruction_code(0x1c, s, d);
    }
    
    void notb(R s, R d)
    {
        cram.at(pc++) = instruction_code(0x33, s, d);
    }
    
    void orb(R s, R d)
    {
        cram.at(pc++) = instruction_code(0x31, s, d);
    }
    
    void popb()
    {
        cram.at(pc++) = instruction_code(0x14, R(0), R(0));
    }
    
    void popb(R d)
    {
        cram.at(pc++) = instruction_code(0x14, R(1), d);
    }
    
    void popd()
    {
        cram.at(pc++) = instruction_code(0x17, R(0), R(0));
    }
    
    void popd(R d)
    {
        cram.at(pc++) = instruction_code(0x17, R(1), d);
    }
    
    void poph()
    {
        cram.at(pc++) = instruction_code(0x15, R(0), R(0));
    }
    
    void poph(R d)
    {
        cram.at(pc++) = instruction_code(0x15, R(1), d);
    }
    
    void pops()
    {
        cram.at(pc++) = instruction_code(0x19, R(0), R(0));
    }
    
    void pops(R d)
    {
        cram.at(pc++) = instruction_code(0x19, R(1), d);
    }
    
    void popw()
    {
        cram.at(pc++) = instruction_code(0x16, R(0), R(0));
    }
    
    void popw(R d)
    {
        cram.at(pc++) = instruction_code(0x16, R(1), d);
    }
    
    void pushb()
    {
        cram.at(pc ++) = instruction_code(0x24, R(0), R(0));
    }
    
    void pushb(R s)
    {
        cram.at(pc ++) = instruction_code(0x24, s, R(1));
    }
    
    void sori(Imme imme, R d)
    {
        Memory::byte_type code = 0xe0;
        code |= imme << 1;
        code |= d;
        cram.at(pc++) = code;
    }
    
    void sys(R s, R d)
    {
        cram.at(pc++) = instruction_code(0x00, s, d);
    }
    
    void xorb(R s, R d)
    {
        cram.at(pc++) = instruction_code(0x32, s, d);
    }

private:
    Memory::size_type pc {0};
    Address data {0};
    Memory& cram;
    Bus64& dram;
}; // class Assembler

Environment::Environment(unsigned bits) : cpu(bits)
{
    Assembler as {cpu.code_ram(), cpu.data_ram()};
    as.sori(Imme(0x1), R(0));
    as.sori(Imme(0xa), R(0));
    as.sori(Imme(0x7), R(0));
    as.sori(Imme(0xe), R(0));
    as.sori(Imme(0x4), R(1));
    as.sori(Imme(0x5), R(1));
    as.sori(Imme(0x6), R(1));
    as.sori(Imme(0x7), R(1));
    as.andb(R(0), R(1));
    as.xorb(R(1), R(1));
    as.xorb(R(0), R(0));
    as.sys(R(1), R(0));
}

void Environment::start()
{
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
