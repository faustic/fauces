// parser.cpp
// Virtual system instruction parser
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

#include "parser.hpp"

#include "using_cstdint.hpp"

namespace vs
{

using Opcode_parser = void (*)(Runner&, uint_least8_t);

static void unimplemented(Runner& runner, uint_least8_t instruction)
{
    runner.unimplemented();
}

static void call(Runner& runner, uint_least8_t instruction)
{
    if ((instruction >> 1) & 1)
        runner.unimplemented();
    else
        runner.call( R(instruction));
}

static void jmp(Runner& runner, uint_least8_t instruction)
{
    if ((instruction >> 1) & 1)
        runner.unimplemented();
    else
        runner.jmp( R(instruction));
}

static void jmpnz(Runner& runner, uint_least8_t instruction)
{
    runner.jmpnz(R(instruction >> 1), R(instruction));
}

static void jmpz(Runner& runner, uint_least8_t instruction)
{
    runner.jmpz(R(instruction >> 1), R(instruction));
}

static void lb(Runner& runner, uint_least8_t instruction)
{
    runner.lb(R(instruction >> 1), R(instruction));
}

static void ld(Runner& runner, uint_least8_t instruction)
{
    runner.ld(R(instruction >> 1), R(instruction));
}

static void lh(Runner& runner, uint_least8_t instruction)
{
    runner.lh(R(instruction >> 1), R(instruction));
}

static void lw(Runner& runner, uint_least8_t instruction)
{
    runner.lw(R(instruction >> 1), R(instruction));
}

static void ret(Runner& runner, uint_least8_t instruction)
{
    if (instruction & 3)
        runner.unimplemented();
    else
        runner.ret();
}

static void sys(Runner& runner, uint_least8_t instruction)
{
    runner.sys(R(instruction >> 1), R(instruction));
}

static Opcode_parser opcodes[64] =
{
    sys, jmp, call, ret,
    jmpnz, jmpz, unimplemented, unimplemented,
    unimplemented, unimplemented, unimplemented, unimplemented,
    unimplemented, unimplemented, unimplemented, unimplemented,
    
    lb, lh, lw, ld,
    unimplemented, unimplemented, unimplemented, unimplemented,
    unimplemented, unimplemented, unimplemented, unimplemented,
    unimplemented, unimplemented, unimplemented, unimplemented,
    
    unimplemented, unimplemented, unimplemented, unimplemented,
    unimplemented, unimplemented, unimplemented, unimplemented,
    unimplemented, unimplemented, unimplemented, unimplemented,
    unimplemented, unimplemented, unimplemented, unimplemented,
    
    unimplemented, unimplemented, unimplemented, unimplemented,
    unimplemented, unimplemented, unimplemented, unimplemented,
    unimplemented, unimplemented, unimplemented, unimplemented,
    unimplemented, unimplemented, unimplemented, unimplemented
};

void parse_instruction(Runner& runner, uint_least8_t instruction)
{
    opcodes[(instruction >> 2)& 0x3f](runner, instruction);
}

}

