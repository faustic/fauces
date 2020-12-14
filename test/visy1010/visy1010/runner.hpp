// runner.hpp
// Base class for virtual system runners
// Intended compatibility: c++17
//
// Created by Alejandro Castro Garcia on 10 December 2020
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


#ifndef fauvisy_runner_hpp
#define fauvisy_runner_hpp

#include <cstdint>
#include <cstddef>

namespace vs
{

class R
{
public:
    explicit R(unsigned n) : n {n & 1}
    {}
    operator unsigned() const noexcept
    {
        return n;
    }
private:
    unsigned n;
};

class S
{
public:
    explicit S(unsigned n) : n {n & 1}
    {}
    operator unsigned()
    {
        return n;
    }
private:
    unsigned n;
};

class Imme
{
public:
    explicit Imme(unsigned n) : n {n & 0xf}
    {}
    operator unsigned()
    {
        return n;
    }
private:
    unsigned n;
};


struct Unimplemented_instruction_error
{
    std::size_t position;
    std::uint_least8_t instruction;

    Unimplemented_instruction_error(std::uint_least8_t instruction,
                                                        std::size_t position) :
    instruction {instruction}, position {position}
    {}
};

class Runner
{
public:
    virtual ~Runner() = default;
    
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
    
    virtual void lb(R s, R d)
    {
        unimplemented();
    }
    
    virtual void ld(R s, R d)
    {
        unimplemented();
    }
    
    virtual void least(R s, R d)
    {
        unimplemented();
    }
    
    virtual void lh(R s, R d)
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
    
    virtual void ls(R s, S d)
    {
        unimplemented();
    }
    
    virtual void lsr(S s, R d)
    {
        unimplemented();
    }
    
    virtual void lw(R s, R d)
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
    
    virtual void stb(R s, R d)
    {
        unimplemented();
    }
    
    virtual void std(R s, R d)
    {
        unimplemented();
    }
    
    virtual void sth(R s, R d)
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
    
    virtual void sts(S s, R d)
    {
        unimplemented();
    }
    
    virtual void stsr(S s, R d)
    {
        unimplemented();
    }
    
    virtual void stw(R s, R d)
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

#endif /* fauvisy_runner_hpp */
