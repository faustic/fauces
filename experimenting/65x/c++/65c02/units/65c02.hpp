// Frequently used types
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


#ifndef w65c02_hpp
#define w65c02_hpp

#include <faulib/format/memory.hpp>

#include <cstdlib>
#include <cstdint>
#include <vector>
#include <string>
#include <span>
#include <unordered_map>
#include <stdexcept>
#include <atomic>
#include <thread>
#include <chrono>
#include <bit>

namespace w65c02
{
using std::vector;
using std::string;
using std::size_t;
using std::span;
using std::unordered_map;
using std::runtime_error;
using std::atomic;
using std::thread;
using std::chrono::nanoseconds;
using std::chrono::time_point;
using std::chrono::steady_clock;
namespace this_thread = std::this_thread;

using Address = std::uint_least16_t;
using Byte = std::uint8_t;
using Sbyte = std::int8_t;
using Word = std::uint16_t;
using Sword = std::int16_t;
using Long = std::uint32_t;
using Slong = std::int32_t;
using Quad = std::uint64_t;
using Squad = std::int64_t;

inline Sbyte as_signed(Byte v)
{
    return std::bit_cast<Sbyte>(v);
}

inline Sword as_signed(Word v)
{
    return std::bit_cast<Sword>(v);
}

inline Slong as_signed(Long v)
{
    return std::bit_cast<Slong>(v);
}

inline Squad as_signed(Quad v)
{
    return std::bit_cast<Squad>(v);
}

using Mem = faulib::format::Memory_base<Address, Byte, Word, Long, Quad>;

}

#endif /* w65c02_hpp */
