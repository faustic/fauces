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

#include "processor.hpp"
#include "parser.hpp"
#include "memory.hpp"

namespace vs
{

class Disassembler : public Processor
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
        disas << "byte 0x" << std::setw(2) << (unsigned)instruction;
    }
    
    void andb(R s, R d) override
    {
        disas << "and r" << s << ", r" << d;
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

    void least(R s, R d) override
    {
        disas << "least r" << s << ", r" << d;
    }

    void lmb(R s, R d) override
    {
        disas << "lmb r" << s << ", r" << d;
    }

    void lmd(R s, R d) override
    {
        disas << "lmd r" << s << ", r" << d;
    }

    void lmh(R s, R d) override
    {
        disas << "lmh r" << s << ", r" << d;
    }

    void lmw(R s, R d) override
    {
        disas << "lmw r" << s << ", r" << d;
    }
    
    void lrr(R s, R d) override
    {
        disas << "lrr r" << s << ", r" << d;
    }

    void lrs(R s, S d) override
    {
        disas << "lrs r" << s << ", s" << d;
    }

    void lrx(R s, X d) override
    {
        disas << "lrx r" << s << ", x" << d;
    }

    void lsr(S s, R d) override
    {
        disas << "lsr s" << s << ", r" << d;
    }

    void notb(R s, R d) override
    {
        disas << "notb r" << s << ", r" << d;
    }
    
    void orb(R s, R d) override
    {
        disas << "or r" << s << ", r" << d;
    }
    
    void popb() override
    {
        disas << "popb";
    }

    void popb(R d) override
    {
        disas << "popb r" << d;
    }

    void popd() override
    {
        disas << "popd";
    }

    void popd(R d) override
    {
        disas << "popd r" << d;
    }

    void poph() override
    {
        disas << "poph";
    }

    void poph(R d) override
    {
        disas << "poph r" << d;
    }

    void pops() override
    {
        disas << "pops";
    }

    void pops(S d) override
    {
        disas << "pops s" << d;
    }

    void popw() override
    {
        disas << "popw";
    }

    void popw(R d) override
    {
        disas << "popw r" << d;
    }

    void pushb() override
    {
        disas << "pushb";
    }

    void pushb(R d) override
    {
        disas << "pushb r" << d;
    }

    void pushd() override
    {
        disas << "pushd";
    }

    void pushd(R d) override
    {
        disas << "pushd r" << d;
    }

    void pushh() override
    {
        disas << "pushh";
    }

    void pushh(R d) override
    {
        disas << "pushh r" << d;
    }

    void pushs() override
    {
        disas << "pushs";
    }

    void pushs(S d) override
    {
        disas << "pushs s" << d;
    }

    void pushw() override
    {
        disas << "pushw";
    }

    void pushw(R d) override
    {
        disas << "pushw r" << d;
    }

    void ret() override
    {
        disas << "ret";
    }
    
    void shl(R s, R d) override
    {
        disas << "shl r" << s << ", r" << d;
    }

    void shr(R s, R d) override
    {
        disas << "shr r" << s << ", r" << d;
    }

    void sori(Imme imme, R d) override
    {
        disas << "sori 0x" << imme << ", r" << d;
    }

    void stmb(R s, R d) override
    {
        disas << "stmb r" << s << ", r" << d;
    }

    void stmd(R s, R d) override
    {
        disas << "stmd r" << s << ", r" << d;
    }

    void stmh(R s, R d) override
    {
        disas << "stmh r" << s << ", r" << d;
    }

    void stmw(R s, R d) override
    {
        disas << "stmw r" << s << ", r" << d;
    }

    void strr(R s, R d) override
    {
        disas << "strr r" << s << ", r" << d;
    }

    void strs(R s, S d) override
    {
        disas << "strs r" << s << ", s" << d;
    }

    void stsr(S s, R d) override
    {
        disas << "stsr s" << s << ", r" << d;
    }

    void sys(R s, R d) override
    {
        disas << "sys r" << s << ", r" << d;
    }
    
    void xorb(R s, R d) override
    {
        disas << "xor r" << s << ", r" << d;
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
