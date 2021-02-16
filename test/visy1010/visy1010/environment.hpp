// environment.hpp
// Virtual system environment
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


#ifndef fauvisy_environment_hpp
#define fauvisy_environment_hpp

#include "cpu.hpp"

namespace vs
{

struct Unsupported_format
{
};

struct Unsupported_trap
{
    std::uint_least64_t code;
    std::uint_least64_t param;
    
    Unsupported_trap(std::uint_least64_t code, std::uint_least64_t param) :
    code {code}, param {param}
    {}
};

class Environment
{
public:
    Environment(unsigned bits);
    void start(int argc, char** argv);
    std::uint_least16_t result()
    {
        return leave_code;
    }
    
private:
    Cpu cpu;
    bool on = false;
    std::uint_least16_t leave_code = 0xffff;
    
    static int constexpr handler_max = 0xf;
    
    using Trap_handler = void (Environment::*)(std::uint_least64_t code,
                                                    std::uint_least64_t param);
    static Trap_handler handlers[handler_max + 1];

    void system_trap(R s, R d);
    void unsupported(std::uint_least64_t code, std::uint_least64_t param);
    
    void leave(std::uint_least64_t code, std::uint_least64_t param);
};

} // namespace vs

#endif /* fauvisy_environment_hpp */
