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

namespace  vs
{

class Memory
{
public:
    virtual ~Memory() = default;
    virtual std::uint_least8_t& operator [](std::size_t n) = 0;
    virtual std::uint_least8_t& at(std::size_t n) = 0;
};

class Simple_memory: public Memory
{
public:
    Simple_memory(std::size_t size) : data(size)
    {}
    std::uint_least8_t& operator [](std::size_t n) override
    {
        return data[n];
    }
    std::uint_least8_t& at(std::size_t n) override
    {
        return data.at(n);
    }
protected:
    std::vector<std::uint_least8_t> data;
};

}

#endif /* fauvisy_memory_hpp */
