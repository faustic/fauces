// exec.hpp
// Reading of executable test program: interface
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


#ifndef w65c02sxb_exec_hpp
#define w65c02sxb_exec_hpp

#include <stdexcept>

#include "sxb.hpp"

namespace w65c02
{
using std::runtime_error;

class Error_exec_small: public runtime_error
{
public:
    Error_exec_small(): runtime_error("w65c02: Error_exec_small") {}
};

class Error_exec_signature: public runtime_error
{
public:
    Error_exec_signature(): runtime_error("w65c02: Error_exec_signature") {}
};

class Error_exec_unexpected_code_start: public runtime_error
{
public:
    Error_exec_unexpected_code_start():
    runtime_error("w65c02: Error_exec_unexpected_code_start") {}
};

class Error_exec_unexpected_code_size: public runtime_error
{
public:
    Error_exec_unexpected_code_size():
    runtime_error("w65c02: Error_exec_unexpected_code_size") {}
};

class Error_exec_unexpected_result_start: public runtime_error
{
public:
    Error_exec_unexpected_result_start():
    runtime_error("w65c02: Error_exec_unexpected_result_start") {}
};

class Error_exec_unexpected_result_size: public runtime_error
{
public:
    Error_exec_unexpected_result_size():
    runtime_error("w65c02: Error_exec_unexpected_result_size") {}
};

class Executable
{
public:
    static constexpr unsigned code_start_pos = 16;
    static constexpr unsigned code_size_pos = 18;
    static constexpr unsigned result_start_pos = 20;
    static constexpr unsigned result_size_pos = 22;
    static constexpr unsigned code_pos = 32;
    Executable(const char* path);
    vector<Byte>& get_bytes()
    {
        return data;
    }
    
    Address code_start()
    {
        unsigned value = read16(code_start_pos);
        if (value < 0x2000)
            throw Error_exec_unexpected_code_start();
        return Address {value};
    }
    
    Size code_size()
    {
        unsigned long value = read16(code_size_pos);
        if (value < 1 || value > 0x5000 || (code_start() + value > 0x7000))
            throw Error_exec_unexpected_code_size();
        return Size {static_cast<unsigned>(value)};
    }
    
    Size size()
    {
        return Size(data.size() & 0xffff);
    }
    
    Address result_start()
    {
        unsigned value = read16(result_start_pos);
        if (value < 0x2000)
            throw Error_exec_unexpected_result_start();
        return Address {value};
    }
    
    Size result_size()
    {
        unsigned value = read16(result_size_pos);
        if (value > 0x5000 || (code_start() + value > 0x7000))
            throw Error_exec_unexpected_result_size();
        return Size {value};
    }
private:
    vector<Byte> data;
    static constexpr array<Byte, 16> signature
    {
        0x3A, 0x8D, 0x90, 0xF6, 0xF9, 0x92, 0x40, 0x45,
        0x83, 0xB7, 0xE6, 0xAE, 0x62, 0x03, 0x38, 0x34
    };
    unsigned read16(size_t addr)
    {
        return data.at(addr) | (data.at(addr + 1) << 8);
    }
};

} // namespace w65c02

#endif /* w65c02sxb_exec_hpp */
