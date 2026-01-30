//  memory.hpp
//  Integer storage in arrangements of bytes
//
//  Created by Alejandro Castro García on 1 July 2020.
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

//

/*

# EXPLANATION OF INTENT
 
We often need to manipulate integer types that are stored within an ordered
sequence of bytes following certain conventions. The sequence of bytes may
represent different things, such as the memory of a virtual machine or a data
file loaded from disk.
 
This header provides some abstract classes that may assist programs in using
a common interface to manipulate integers regardless of the conventions
followed to store the integers.
 
*/

#ifndef faulib_format_memory_hpp
#define faulib_format_memory_hpp

#include <cstddef>

namespace faulib::format
{

template<typename A, typename B8, typename B16, typename B32, typename B64>
class Memory_base
{
public:
    virtual ~Memory_base() = default;
        
    virtual B8 read8(A addr) = 0;
    virtual B16 read16(A addr) = 0;
    virtual B32 read32(A addr) = 0;
    virtual B64 read64(A addr) = 0;
    
    virtual void write8(A addr, B8 value) = 0;
    virtual void write16(A addr, B16 value) = 0;
    virtual void write32(A addr, B32 value) = 0;
    virtual void write64(A addr, B64 value) = 0;
    
    virtual void copybytes(unsigned char* dst, std::size_t size) = 0;
};

template<typename A, typename B8, typename B16, typename B32, typename B64>
class Memory_be: public Memory_base<A, B8, B16, B32, B64>
{
public:
    B16 read16(A addr) override
    {
        B16 part0 = static_cast<B16>(this->read8(addr)) << 8;
        return part0 | static_cast<B16>(this->read8(addr + 1));
    }

    B32 read32(A addr) override
    {
        B32 part0 = static_cast<B32>(this->read16(addr)) << 16;
        return part0 | static_cast<B32>(this->read16(addr + 2));
    }

    B64 read64(A addr) override
    {
        B64 part0 = static_cast<B64>(this->read32(addr)) << 32;
        return part0 | static_cast<B64>(this->read32(addr + 4));
    }
    
    void write16(A addr, B16 value) override
    {
        this->write8(addr, static_cast<B8>(value >> 8));
        this->write8(addr + 1, static_cast<B8>(value));
    }

    void write32(A addr, B32 value) override
    {
        this->write16(addr, static_cast<B16>(value >> 16));
        this->write16(addr + 2, static_cast<B16>(value));
    }

    void write64(A addr, B64 value) override
    {
        this->write32(addr, static_cast<B32>(value >> 32));
        this->write32(addr + 4, static_cast<B32>(value));
    }
};

template<typename A, typename B8, typename B16, typename B32, typename B64>
class Memory_le: public Memory_base<A, B8, B16, B32, B64>
{
public:
    B16 read16(A addr) override
    {
        B16 part0 = static_cast<B16>(this->read8(addr));
        return (static_cast<B16>(this->read8(addr + 1)) << 8) | part0;
    }

    B32 read32(A addr) override
    {
        B32 part0 = static_cast<B32>(this->read16(addr));
        return (static_cast<B32>(this->read16(addr + 2)) << 16) | part0;
    }

    B64 read64(A addr) override
    {
        B64 part0 = static_cast<B64>(this->read32(addr));
        return (static_cast<B64>(this->read32(addr + 4)) << 32) | part0;
    }
    
    void write16(A addr, B16 value) override
    {
        this->write8(addr, static_cast<B8>(value));
        this->write8(addr + 1, static_cast<B8>(value >> 8));
    }

    void write32(A addr, B32 value) override
    {
        this->write16(addr, static_cast<B16>(value));
        this->write16(addr + 2, static_cast<B16>(value >> 16));
    }

    void write64(A addr, B64 value) override
    {
        this->write32(addr, static_cast<B32>(value));
        this->write32(addr + 4, static_cast<B32>(value >> 32));
    }
};

} // namespace faulib::format


#endif  // faulib_format_memory_hpp
