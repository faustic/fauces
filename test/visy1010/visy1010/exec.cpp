// exec.cpp
// Loading of executable file
// Intended compatibility: c++17
//
// Created by Alejandro Castro García on 22 February 2021
/*
Licensed under the MIT License.
 
Copyright (c) 2021 Faustic Inferno SL
 
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


#include "exec.hpp"

namespace vs
{
static const unsigned char exe_uuid[16] =
{
    0xec, 0x4b, 0x79, 0xc8, 0x57, 0xee, 0x4b, 0xad,
    0x96, 0xcf, 0x93, 0x85, 0x48, 0xa2, 0xe0, 0x4b
};

enum class Exe_addr
{
    undefined = 0xffff
};

struct Exe
{
    unsigned char uuid[16];
    unsigned short cpu;
    unsigned short start_section;
    unsigned short start_addr;
};

struct Section_header
{
    unsigned short id;
    unsigned short type;
    unsigned short size;
    unsigned short pref_start;
    unsigned short relocs;
};

struct Code_section
{
    Section_header header;
    unsigned short code_size;
};

Program::Program(char* filename)
{
    
}

}
