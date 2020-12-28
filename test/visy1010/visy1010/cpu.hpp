// cpu.hpp
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


#ifndef fauvisy_cpu_hpp
#define fauvisy_cpu_hpp

#include <cstdint>

#include "processor.hpp"
#include "memory.hpp"
#include "parser.hpp"

namespace vs
{

class Cpu: public Processor
{
public:
    Cpu(unsigned bits) :
    cram(1ULL << bits),
    dram(1ULL << bits),
    pm((1ULL << bits) - 1)
    {}
    
    void reset()
    {
        pc = 0;
        r[0] = r[1] = 0;
        s[0] = s[1] = 0;
        s[0] = s[1] = 0;
    }
    
    void loop()
    {
        running = true;
        while (running)
        {
            std::uint_least64_t instruction_pc = pc;
            ++pc;
            parse_instruction(*this, cram[instruction_pc & pm]);
        }
    }
    
    void stop()
    {
        running = false;
    }
    
    Memory& code_ram()
    {
        return cram;
    }
    
    Bus64& data_ram()
    {
        return dram;
    }
    
    std::uint_least64_t& program_counter()
    {
        return pc;
    }
    
    std::uint_least64_t& r0()
    {
        return r[0];
    }
    
    std::uint_least64_t& r1()
    {
        return r[1];
    }
    
    std::uint_least64_t& s0()
    {
        return s[0];
    }
    
    std::uint_least64_t& s1()
    {
        return s[1];
    }
    
    std::uint_least64_t& x0()
    {
        return x[0];
    }
    
    std::uint_least64_t& x1()
    {
        return x[1];
    }

private:
    void unimplemented() override
    {
        std::size_t addr = static_cast<std::size_t>((pc - 1) & pm);
        throw Unimplemented_instruction_error(cram[addr], addr);
    }
    
    void andb(R s, R d) override
    {
        r[d] &= r[s];
    }
    
    void sori(Imme imme, R d) override
    {
        r[d] <<= 4;
        r[d] |= imme;
    }
    
    void sys(R s, R d) override
    {
        throw System_trap(s, d);
    }
    
    void xorb(R s, R d) override
    {
        r[d] ^= r[s];
    }
    
    void execute()
    {
        while (running)
        {
            
        }
    }
    
    Simple_memory cram;
    Memory_be dram;
    std::uint_least64_t pm;
    std::uint_least64_t pc = 0;
    std::uint_least64_t r[2] = {0, 0};
    std::uint_least64_t s[2] = {0, 0};
    std::uint_least64_t x[2] = {0, 0};
    bool running = false;
}; // class Cpu

} // namespace vs

#endif /* fauvisy_cpu_hpp */
