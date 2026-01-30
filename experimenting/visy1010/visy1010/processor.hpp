// processor.hpp
// Base class for virtual system processors
//
// Created by Alejandro Castro Garcia on 10 December 2020
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


#ifndef fauvisy_processor_hpp
#define fauvisy_processor_hpp

#include <cstdint>
#include <cstddef>

#include "arch.hpp"

namespace vs
{

struct Unimplemented_instruction_error
{
    std::size_t position;
    std::uint_least8_t instruction;

    Unimplemented_instruction_error(std::uint_least8_t instruction,
                                                        std::size_t position) :
    instruction {instruction}, position {position}
    {}
};

struct System_trap
{
    R s;
    R d;
    
    System_trap(R s, R d) : s {s}, d {d}
    {}
};

class Processor
{
public:
    virtual ~Processor() = default;
    
    virtual void unimplemented() = 0;
    
    virtual void andb(R s, R d)
    {
        unimplemented();
    }
    
    virtual void call(R d)
    {
        unimplemented();
    }
    
    virtual void jmp(R d)
    {
        unimplemented();
    }

    virtual void jmpnz(R s, R d)
    {
        unimplemented();
    }
    
    virtual void jmpz(R s, R d)
    {
        unimplemented();
    }
    
    virtual void least(R s, R d)
    {
        unimplemented();
    }
    
    virtual void lmb(R s, R d)
    {
        unimplemented();
    }
    
    virtual void lmd(R s, R d)
    {
        unimplemented();
    }
    
    virtual void lmh(R s, R d)
    {
        unimplemented();
    }
    
    virtual void lmw(R s, R d)
    {
        unimplemented();
    }
    
    virtual void lrr(R s, R d)
    {
        unimplemented();
    }
    
    virtual void lrs(R s, S d)
    {
        unimplemented();
    }
    
    virtual void lrx(R s, X d)
    {
        unimplemented();
    }
    
    virtual void lsr(S s, R d)
    {
        unimplemented();
    }
    
    virtual void notb(R s, R d)
    {
        unimplemented();
    }
    
    virtual void orb(R s, R d)
    {
        unimplemented();
    }
    
    virtual void popb()
    {
        unimplemented();
    }
    
    virtual void popb(R d)
    {
        unimplemented();
    }
    
    virtual void popd()
    {
        unimplemented();
    }
    
    virtual void popd(R d)
    {
        unimplemented();
    }
    
    virtual void poph()
    {
        unimplemented();
    }
    
    virtual void poph(R d)
    {
        unimplemented();
    }
    
    virtual void pops()
    {
        unimplemented();
    }
    
    virtual void pops(S d)
    {
        unimplemented();
    }
    
    virtual void popw()
    {
        unimplemented();
    }
    
    virtual void popw(R d)
    {
        unimplemented();
    }
    
    virtual void pushb()
    {
        unimplemented();
    }
    
    virtual void pushb(R d)
    {
        unimplemented();
    }
    
    virtual void pushd()
    {
        unimplemented();
    }
    
    virtual void pushd(R d)
    {
        unimplemented();
    }
    
    virtual void pushh()
    {
        unimplemented();
    }
    
    virtual void pushh(R d)
    {
        unimplemented();
    }
    
    virtual void pushs()
    {
        unimplemented();
    }
    
    virtual void pushs(S d)
    {
        unimplemented();
    }
    
    virtual void pushw()
    {
        unimplemented();
    }
    
    virtual void pushw(R d)
    {
        unimplemented();
    }
    
    virtual void ret()
    {
        unimplemented();
    }

    virtual void shl(R s, R d)
    {
        unimplemented();
    }
    
    virtual void shr(R s, R d)
    {
        unimplemented();
    }
    
    virtual void sori(Imme imme, R d)
    {
        unimplemented();
    }
    
    virtual void stmb(R s, R d)
    {
        unimplemented();
    }
    
    virtual void stmd(R s, R d)
    {
        unimplemented();
    }
    
    virtual void stmh(R s, R d)
    {
        unimplemented();
    }
    
    virtual void stmw(R s, R d)
    {
        unimplemented();
    }
    
    virtual void strr(R s, R d)
    {
        unimplemented();
    }
    
    virtual void strs(R s, S d)
    {
        unimplemented();
    }
    
    virtual void stsr(S s, R d)
    {
        unimplemented();
    }
    
    virtual void sys(R s, R d)
    {
        unimplemented();
    }
    
    virtual void xorb(R s, R d)
    {
        unimplemented();
    }
    
};

}

#endif /* fauvisy_processor_hpp */
