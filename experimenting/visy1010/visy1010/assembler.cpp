// assembler.cpp
// Virtual system assembler
// Intended compatibility: c++17
//
// Created by Alejandro Castro Garcia on 28 December 2020
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


#include "assembler.hpp"

#include "using_string.hpp"
#include "using_cstdint.hpp"


namespace vs
{

void Assembler::call(const string& label16, R d)
{
    use_label(label16, d);
    call(d);
}

void Assembler::end()
{
    if (!future_labels.empty())
        throw Error_undefined_label {future_labels.begin()->first};
    ended = true;
}

void Assembler::jmp(const string& label16, R d)
{
    use_label(label16, d);
    jmp(d);
}

void Assembler::jmpnz(R s, const string& label16, R d)
{
    use_label(label16, d);
    jmpnz(s, d);
}

void Assembler::jmpz(R s, const string& label16, R d)
{
    use_label(label16, d);
    jmpz(s, d);
}

void Assembler::label(const string& label16)
{
    if (labels.find(label16) != labels.end())
        throw Error_duplicate_label {label16};
    auto label_pc = pc;
    labels.insert({label16, label_pc});
    for (auto i = future_labels.begin(); i != future_labels.end(); ++i)
    {
        for (auto j = i->second.begin(); j != i->second.end(); ++j)
        {
            pc = *j;
            use_label(static_cast<unsigned>(label_pc), R {cram.at(pc)});
        }
    }
    future_labels.erase(label16);
    pc = label_pc;
}

void Assembler::lid(uint_least64_t value, R d)
{
    if ((value & 0xffff'ffff'0000'0000) == 0xffff'ffff'0000'0000)
    {
        xorb(d, d);
        notb(d, d);
        if ((value & 0xf000'0000) != 0xf000'0000)
            sorix(8, value, d);
        else if ((value & 0xff00'0000) != 0xff00'0000)
            sorix(7, value, d);
        else if ((value & 0xfff0'0000) != 0xfff0'0000)
            sorix(6, value, d);
        else if ((value & 0xffff'0000) != 0xffff'0000)
            sorix(5, value, d);
        else if ((value & 0xffff'f000) != 0xffff'f000)
            sorix(4, value, d);
        else if ((value & 0xffff'ff00) != 0xffff'ff00)
            sorix(3, value, d);
        else if ((value & 0xffff'fff0) != 0xffff'fff0)
            sorix(2, value, d);
        else
            sorix(1, value, d);
    }
    else if (value & 0xff00'0000'0000'0000)
        sorix(16, value, d);
    else if (value & 0xffff'ffff'0000'0000)
    {
        xorb(d, d);
        if (value & 0xfff0'0000'0000'0000)
            sorix(14, value, d);
        else if (value & 0xffff'0000'0000'0000)
            sorix(13, value, d);
        else if (value & 0xffff'f000'0000'0000)
            sorix(12, value, d);
        else if (value & 0xffff'ff00'0000'0000)
            sorix(11, value, d);
        else if (value & 0xffff'fff0'0000'0000)
            sorix(10, value, d);
        else
            sorix(9, value, d);
    }
    else
        liwz(static_cast<uint_least32_t>(value), d);
}

void Assembler::liw(uint_least32_t value, R d)
{
    if (value & 0xff00'0000)
        sorix(8, value, d);
    else if (value & 0xffff'0000)
    {
        xorb(d, d);
        if (value & 0xfff0'0000)
            sorix(6, value, d);
        else
            sorix(5, value, d);
    }
    else
        lihz(static_cast<uint_least16_t>(value), d);
}

void Assembler::liwz(uint_least32_t value, R d)
{
    if (value & 0xffff'0000)
    {
        xorb(d, d);
        if (value & 0xf000'0000)
            sorix(8, value, d);
        else if (value & 0xff00'0000)
            sorix(7, value, d);
        else if (value & 0xfff0'0000)
            sorix(6, value, d);
        else
            sorix(5, value, d);
    }
    else
        lihz(static_cast<uint_least16_t>(value), d);
}

void Assembler::lih(uint_least16_t value, R d)
{
    if (value & 0xff00)
        sorix(4, value, d);
    else
    {
        xorb(d, d);
        if (value & 0xfff0)
            sorix(2, value, d);
        else if (value & 0xffff)
            sori(Imme(value), d);
    }
}

void Assembler::lihz(uint_least16_t value, R d)
{
    xorb(d, d);
    if (value & 0xf000)
        sorix(4, value, d);
    else if (value & 0xff00)
        sorix(3, value, d);
    else if (value & 0xfff0)
        sorix(2, value, d);
    else if (value & 0xffff)
        sori(Imme(value), d);
}

void Assembler::sorix(unsigned nibbles, std::uint_least64_t value, R d)
{
    if (nibbles > 16)
        throw Error_bad_nibble_number {nibbles};
    while (nibbles--)
    {
        unsigned bits = nibbles * 4;
        sori(Imme(static_cast<unsigned>(value >> bits)), d);
    }
}


void Assembler::use_label(unsigned addr, R d)
{
    sori(Imme(addr >> 12), d);
    sori(Imme(addr >> 8), d);
    sori(Imme(addr >> 4), d);
    sori(Imme(addr), d);
}

void Assembler::use_label(string label16, R d)
{
    unsigned addr = 0;
    auto entry = labels.find(label16);
    if (entry != labels.end())
        addr = static_cast<unsigned>(entry->second);
    else
        future_labels[label16].push_back(pc);
    use_label(addr, d);
}

};
