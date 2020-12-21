// main.cpp
// Entry to virtual system launcher
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

#include "disassembler.hpp"
#include "using_cstdint.hpp"
#include "using_iostream.hpp"
#include "using_string.hpp"

using namespace vs;

int main()
{
    Simple_memory mem(256);
    for (size_t i = 0; i < 256; ++i)
        mem[i] = static_cast<uint_least8_t>(i);
    try
    {
        cout << static_cast<string>(Disassembler {mem, 0, 256}) ;
    }
    catch(Unimplemented_instruction_error e)
    {
        cout << "Illegal instruction 0x" << hex << setfill('0') << setw(2) <<
                (unsigned)e.instruction << " at 0x" << setw(4) <<
                e.position << '\n';
        cout << "Opcode: 0x" << setw(2) << (e.instruction >> 2) << dec << '\n';
    }
}

