// Loading of executable
//
// Created by Alejandro Castro García on 8 May 2023
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


#ifndef w65c02_executable_hpp
#define w65c02_executable_hpp

#include "65c02.hpp"

#include <faulib/io/rawfile.hpp>
#include <algorithm>

namespace w65c02
{

class Exec_Error: public runtime_error
{
public:
    Exec_Error(const string& what_arg): runtime_error(what_arg)
    {}
};

class Executable
{
public:
    Executable(Mem& m, Word start = 0, Word size = 0, Address result_start = 0,
               Word result_size = 0):
    mem(m),
    priv_start(start),
    size(size),
    priv_result_start(result_start),
    priv_result_size(result_size)
    {
    }
    
    void load(const string& path)
    {
        using faulib::io::readraw;
        auto bytes = readraw<Byte>(path.c_str());
        if (!check_signature(bytes))
            throw Exec_Error("Bad signature");
        if (!position(bytes))
            throw Exec_Error("Unable to position program in memory");
        if (!load(span<Byte>(bytes.begin() + 32, bytes.end())))
            throw Exec_Error("Unable to load program into memory");
    }
    
    Word result_size()
    {
        return priv_result_size;
    }
    
    Address result_start()
    {
        return priv_result_start;
    }
    
    Address start()
    {
        return priv_start;
    }
    
private:
    Mem& mem;
    Address priv_start;
    Word size;
    Address priv_result_start;
    Word priv_result_size;
    
    static bool check_signature(span<Byte> bytes)
    {
        if (bytes.size() < 32)
            return false;
        for(int i = 0; i != 16; ++i)
        {
            if (bytes[i] != signature[i])
                return false;
        }
        return true;
    }
    
    bool position(span<Byte> bytes)
    {
        priv_start = read_short(bytes, 16);
        if (priv_start == 0)
            priv_start = 4128;
        size = read_short(bytes, 18);
        if (priv_start + size > 0xe000)
            return false;
        priv_result_start = read_short(bytes, 20);
        priv_result_size = read_short(bytes, 22);
        return true;
    }
    
    bool load(span<Byte> bytes)
    {
        if (size != bytes.size())
            return false;
        for (Word i = 0; i != size; ++i)
            mem.write8(priv_start + i, bytes[i]);
        return true;
    }
    
    static Word read_short(span<Byte> bytes, size_t pos)
    {
        return (Word(bytes[pos + 1]) << 8) | bytes[pos];
    }

    static constexpr Byte signature[] =
    {
        0x3A, 0x8D, 0x90, 0xF6, 0xF9, 0x92, 0x40, 0x45,
        0x83, 0xB7, 0xE6, 0xAE, 0x62, 0x03, 0x38, 0x34
    };
};

}

#endif /* w65c02_executable_hpp */
