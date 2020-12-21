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

using Opcode_parser = void (*)(Processor&, uint_least8_t);

static void unimplemented(Processor& processor, uint_least8_t instruction)
{
    processor.unimplemented();
}

static void andb(Processor& processor, uint_least8_t instruction)
{
    processor.andb(R(instruction >> 1), R(instruction));
}

static void call(Processor& processor, uint_least8_t instruction)
{
    if ((instruction >> 1) & 1)
        processor.unimplemented();
    else
        processor.call( R(instruction));
}

static void jmp(Processor& processor, uint_least8_t instruction)
{
    if ((instruction >> 1) & 1)
        processor.unimplemented();
    else
        processor.jmp( R(instruction));
}

static void jmpnz(Processor& processor, uint_least8_t instruction)
{
    processor.jmpnz(R(instruction >> 1), R(instruction));
}

static void jmpz(Processor& processor, uint_least8_t instruction)
{
    processor.jmpz(R(instruction >> 1), R(instruction));
}

static void least(Processor& processor, uint_least8_t instruction)
{
    processor.least(R(instruction >> 1), R(instruction));
}

static void lmb(Processor& processor, uint_least8_t instruction)
{
    processor.lmb(R(instruction >> 1), R(instruction));
}

static void lmd(Processor& processor, uint_least8_t instruction)
{
    processor.lmd(R(instruction >> 1), R(instruction));
}

static void lmh(Processor& processor, uint_least8_t instruction)
{
    processor.lmh(R(instruction >> 1), R(instruction));
}

static void lmw(Processor& processor, uint_least8_t instruction)
{
    processor.lmw(R(instruction >> 1), R(instruction));
}

static void lrr(Processor& processor, uint_least8_t instruction)
{
    processor.lrr(R(instruction >> 1), R(instruction));
}

static void lrs(Processor& processor, uint_least8_t instruction)
{
    processor.lrs(R(instruction >> 1), S(instruction));
}

static void lrx(Processor& processor, uint_least8_t instruction)
{
    processor.lrx(R(instruction >> 1), X(instruction));
}

static void lsr(Processor& processor, uint_least8_t instruction)
{
    processor.lsr(S(instruction >> 1), R(instruction));
}

static void notb(Processor& processor, uint_least8_t instruction)
{
    processor.notb(R(instruction >> 1), R(instruction));
}

static void orb(Processor& processor, uint_least8_t instruction)
{
    processor.orb(R(instruction >> 1), R(instruction));
}

static void popb(Processor& processor, uint_least8_t instruction)
{
    if (instruction & 2)
        processor.popb(R(instruction));
    else
        processor.popb();
}

static void popd(Processor& processor, uint_least8_t instruction)
{
    if (instruction & 2)
        processor.popd(R(instruction));
    else
        processor.popd();
}

static void poph(Processor& processor, uint_least8_t instruction)
{
    if (instruction & 2)
        processor.poph(R(instruction));
    else
        processor.poph();
}

static void pops(Processor& processor, uint_least8_t instruction)
{
    if (instruction & 2)
        processor.pops(S(instruction));
    else
        processor.pops();
}

static void popw(Processor& processor, uint_least8_t instruction)
{
    if (instruction & 2)
        processor.popw(R(instruction));
    else
        processor.popw();
}

static void pushb(Processor& processor, uint_least8_t instruction)
{
    if (instruction & 1)
        processor.pushb(R(instruction >> 1));
    else
        processor.pushb();
}

static void pushd(Processor& processor, uint_least8_t instruction)
{
    if (instruction & 1)
        processor.pushd(R(instruction >> 1));
    else
        processor.pushd();
}

static void pushh(Processor& processor, uint_least8_t instruction)
{
    if (instruction & 1)
        processor.pushh(R(instruction >> 1));
    else
        processor.pushh();
}

static void pushs(Processor& processor, uint_least8_t instruction)
{
    if (instruction & 1)
        processor.pushs(S(instruction >> 1));
    else
        processor.pushs();
}

static void pushw(Processor& processor, uint_least8_t instruction)
{
    if (instruction & 1)
        processor.pushw(R(instruction >> 1));
    else
        processor.pushw();
}

static void ret(Processor& processor, uint_least8_t instruction)
{
    if (instruction & 3)
        processor.unimplemented();
    else
        processor.ret();
}

static void shl(Processor& processor, uint_least8_t instruction)
{
    processor.shl(R(instruction >> 1), R(instruction));
}

static void shr(Processor& processor, uint_least8_t instruction)
{
    processor.shr(R(instruction >> 1), R(instruction));
}

static void sori(Processor& processor, uint_least8_t instruction)
{
    processor.sori(Imme(instruction >> 1), R(instruction));
}

static void stmb(Processor& processor, uint_least8_t instruction)
{
    processor.stmb(R(instruction >> 1), R(instruction));
}

static void stmd(Processor& processor, uint_least8_t instruction)
{
    processor.stmd(R(instruction >> 1), R(instruction));
}

static void stmh(Processor& processor, uint_least8_t instruction)
{
    processor.stmh(R(instruction >> 1), R(instruction));
}

static void stmw(Processor& processor, uint_least8_t instruction)
{
    processor.stmw(R(instruction >> 1), R(instruction));
}

static void strr(Processor& processor, uint_least8_t instruction)
{
    processor.strr(R(instruction >> 1), R(instruction));
}

static void strs(Processor& processor, uint_least8_t instruction)
{
    processor.strs(R(instruction >> 1), S(instruction));
}

static void stsr(Processor& processor, uint_least8_t instruction)
{
    processor.stsr(S(instruction >> 1), R(instruction));
}

static void sys(Processor& processor, uint_least8_t instruction)
{
    processor.sys(R(instruction >> 1), R(instruction));
}

static void xorb(Processor& processor, uint_least8_t instruction)
{
    processor.xorb(R(instruction >> 1), R(instruction));
}

static Opcode_parser opcodes[64] =
{
    sys, jmp, call, ret,
    jmpnz, jmpz, unimplemented, unimplemented,
    unimplemented, unimplemented, unimplemented, unimplemented,
    unimplemented, unimplemented, unimplemented, unimplemented,
    
    lmb, lmh, lmw, lmd,
    popb, poph, popw, popd,
    lrx, pops, lrr, lrs,
    lsr, unimplemented, unimplemented, unimplemented,
    
    stmb, stmh, stmw, stmd,
    pushb, pushh, pushw, pushd,
    unimplemented, pushs, strr, strs,
    stsr, unimplemented, unimplemented, unimplemented,
    
    andb, orb, xorb, notb,
    least, shl, shr, unimplemented,
    sori, sori, sori, sori,
    sori, sori, sori, sori
};

void parse_instruction(Processor& processor, uint_least8_t instruction)
{
    opcodes[(instruction >> 2)& 0x3f](processor, instruction);
}

}

