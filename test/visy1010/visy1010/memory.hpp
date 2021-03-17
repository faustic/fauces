// memory.hpp
// Suitable containers for virtual system memory
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


#ifndef fauvisy_memory_hpp
#define fauvisy_memory_hpp

#include <cstddef>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <iostream>

namespace  vs
{

template <typename Address_type,
            typename Byte_type>
class GenericMemory
{
public:
    using size_type = Address_type;
    using byte_type = Byte_type;
    virtual ~GenericMemory() = default;
    virtual byte_type& operator [](size_type n) = 0;
    virtual byte_type& at(size_type n) = 0;
    virtual void load(const std::vector<byte_type>& ext_data, size_type start)
                                                                            = 0;
};

using Memory =
    GenericMemory<std::vector<uint_least8_t>::size_type, std::uint_least8_t>;

class Simple_memory: public Memory
{
public:
    Simple_memory(size_type size) : data(size)
    {}
    
    byte_type& operator [](size_type n) override
    {
        return data[n];
    }
    
    byte_type& at(size_type n) override
    {
        return data.at(n);
    }
    
    void resize(size_type new_size)
    {
        data.resize(new_size);
    }
    
    void load(const std::vector<byte_type>& ext_data, size_type start) override
    {
        if (start > data.size())
            throw std::out_of_range("Start address out of limits");
        if (ext_data.size() > data.size() - start)
            throw std::out_of_range("Source too big");
        std::copy(ext_data.begin(), ext_data.end(), data.begin() +
            static_cast<std::vector<uint_least8_t>::difference_type> (start));
    }
    
protected:
    std::vector<byte_type> data;
};

template <typename Address_type>
class GenericAddress
{
public:
    GenericAddress() = default;
    
    explicit GenericAddress(Address_type value): value {value}
    {}
    
    operator Address_type()
    {
        return value;
    }

private:
    Address_type value;
};

using Address = GenericAddress<std::vector<std::uint8_t>::size_type>;

class Bus8
{
public:
    virtual ~Bus8() = default;
    virtual uint_least8_t read8(Address addr) = 0;
    virtual void write8(Address addr, std::uint_least8_t value) = 0;
};

class Bus16: public Bus8
{
public:
    virtual std::uint_least16_t read16(Address addr) = 0;
    virtual void write16(Address addr, std::uint_least16_t value) = 0;
};

class Bus32: public Bus16
{
public:
    virtual std::uint_least32_t read32(Address addr) = 0;
    virtual void write32(Address addr, std::uint_least32_t value) = 0;
};

class Bus64: public Bus32
{
public:
    virtual std::uint_least64_t read64(Address addr) = 0;
    virtual void write64(Address addr, std::uint_least64_t value) = 0;
};

class Memory_be: public Simple_memory, public Bus64
{
public:
    Memory_be(size_type s) : Simple_memory(s)
    {}
    std::uint_least8_t read8(Address addr) override
    {
        return data.at(addr);
    }
    
    void write8(Address addr, std::uint_least8_t value) override
    {
        data.at(addr) = value & 0xff;
    }
    
    std::uint_least16_t read16(Address addr) override
    {
        std::uint_least16_t value =
                        static_cast<std::uint_least16_t>(data.at(addr) << 8);
        value |= data.at(addr + 1);
        return value;
    }
    
    void write16(Address addr, std::uint_least16_t value) override
    {
        data.at(addr) = (value >> 8) & 0xff;
        data.at(addr + 1) = value & 0xff;
    }
    
    std::uint_least32_t read32(Address addr) override
    {
        std::uint_least32_t value =
                        static_cast<std::uint_least32_t>(data.at(addr)) << 24;
        value |= static_cast<std::uint_least32_t>(data.at(addr + 1)) << 16;
        value |= static_cast<std::uint_least32_t>(data.at(addr + 2)) << 8;
        value |= data.at(addr + 3);
        return value;
    }
    
    void write32(Address addr, std::uint_least32_t value) override
    {
        data.at(addr) = (value >> 24) & 0xff;
        data.at(addr + 1) = (value >> 16) & 0xff;
        data.at(addr + 2) = (value >> 8) & 0xff;
        data.at(addr + 3) = value & 0xff;
    }
    
    std::uint_least64_t read64(Address addr) override
    {
        std::uint_least64_t value =
                        static_cast<std::uint_least64_t>(data.at(addr)) << 56;
        value |= static_cast<std::uint_least64_t>(data.at(addr + 1)) << 48;
        value |= static_cast<std::uint_least64_t>(data.at(addr + 2)) << 40;
        value |= static_cast<std::uint_least64_t>(data.at(addr + 3)) << 32;
        value |= static_cast<std::uint_least64_t>(data.at(addr + 4)) << 24;
        value |= static_cast<std::uint_least64_t>(data.at(addr + 5)) << 16;
        value |= static_cast<std::uint_least64_t>(data.at(addr + 6)) << 8;
        value |= data.at(addr + 7);
        return value;
    }
    
    void write64(Address addr, std::uint_least64_t value) override
    {
        data.at(addr) = (value >> 56) & 0xff;
        data.at(addr + 1) = (value >> 48) & 0xff;
        data.at(addr + 2) = (value >> 40) & 0xff;
        data.at(addr + 3) = (value >> 32) & 0xff;
        data.at(addr + 4) = (value >> 24) & 0xff;
        data.at(addr + 5) = (value >> 16) & 0xff;
        data.at(addr + 6) = (value >> 8) & 0xff;
        data.at(addr + 7) = value & 0xff;
    }
};

}

#endif /* fauvisy_memory_hpp */
