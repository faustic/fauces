// disassembler.hpp
// Virtual system disassembler
// Intended compatibility: c++17
//
// Created by Alejandro Castro Garcia on 11 December 2020
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


#ifndef fauvisy_disassembler_hpp
#define fauvisy_disassembler_hpp

#include <sstream>
#include <iomanip>

#include "runner.hpp"
#include "parser.hpp"
#include "memory.hpp"

namespace vs
{

class Disassembler : public Runner
{
public:
    Disassembler(Memory & mem, std::size_t start, std::size_t count,
                                                        int address_width = 4) :
    mem {mem}, address_width {4}
    {
        disas << std::hex << std::setfill('0');
        for (position = start; position < count; ++position)
        {
            disas << "0x" << std::setw(address_width) << position << ": ";
            instruction = mem.at(position);
            parse_instruction(*this, instruction);
            disas << '\n';
        }
    }
    
    operator std::string()
    {
        return disas.str();
    }
    
    void unimplemented() override
    {
        if (instruction >= 64)
            throw Unimplemented_instruction_error(instruction, position);
        else
            disas << "byte 0x" << std::setw(2) << (unsigned)instruction;
    }
    
    void call(R d) override
    {
        disas << "call r" << d;
    }
    
    void jmp(R d) override
    {
        disas << "jmp r" << d;
    }
    
    void jmpnz(R s, R d) override
    {
        disas << "jmpnz r" << s << ", r" << d;
    }
    
    void jmpz(R s, R d) override
    {
        disas << "jmpz r" << s << ", r" << d;
    }

    void lb(R s, R d) override
    {
        disas << "lb r" << s << ", r" << d;
    }

    void ld(R s, R d) override
    {
        disas << "ld r" << s << ", r" << d;
    }

    void lh(R s, R d) override
    {
        disas << "lh r" << s << ", r" << d;
    }

    void lw(R s, R d) override
    {
        disas << "lw r" << s << ", r" << d;
    }

    void ret() override
    {
        disas << "ret";
    }
    
    void sys(R s, R d) override
    {
        disas << "sys r" << s << ", r" << d;
    }
    
private:
    std::stringstream disas;
    Memory& mem;
    std::size_t position;
    std::uint_least8_t instruction;
    int address_width;
};

}

#endif /* fauvisy_disassembler_hpp */
