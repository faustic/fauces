// 65C02 code generation
//
// Created by Alejandro Castro García on 9 May 2023
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


#ifndef w65c02_assembler_hpp
#define w65c02_assembler_hpp

#include "65c02.hpp"

namespace w65c02
{

class Assembler_error: public runtime_error
{
public:
    Assembler_error(const string& what_arg): runtime_error(what_arg)
    {}
};

class Abs
{
public:
    Abs(Address value):
    priv_value(value)
    {}
    Address value() const
    {
        return priv_value;
    }
private:
    Address priv_value;
};

class Imm
{
public:
    Imm(Byte value):
    priv_value(value)
    {
        priv_value = value;
    }
    
    Byte value()
    {
        return priv_value;
    }

private:
    Byte priv_value;
};

class Assembler
{
public:
    Assembler(Mem& mem, Address start): mem(mem), pc(start)
    {}
    
    ~Assembler();
    
    Address program_counter()
    {
        return pc;
    }
    
    void close()
    {
        if (future_labels.size() > 0)
            throw Assembler_error("Undefined labels");
    }
    
    void dc_b(int value)
    {
        mem.write8(pc++, value);
    }
        
    void dc_w(int value)
    {
        mem.write16(pc, value);
        pc += 2;
    }
    
    void label(const string& str)
    {
        if (labels.contains(str))
            throw Assembler_error("Duplicate label");
        if (future_labels.contains(str))
        {
            for (Address offset_pos: future_labels[str])
            {
                Address label_pos = pc;
                Address next_opcode_pos = offset_pos + 1;
                Address offset = label_pos - next_opcode_pos;
                if (offset + 128 > 255)
                    throw Assembler_error("Displacement too big");
                mem.write8(offset_pos, offset);
            }
            future_labels.erase(str);
        }
        labels[str] = pc;
    }

    int offset_to(const string& label)
    {
        // Calculates offset to next opcode from label position.
        // On entry, program_counter should point to offset parameter,
        // just before the next opcode.
        Address offset_pos = pc;
        Address next_opcode_pos = offset_pos + 1;
        int offset = 0;
        if (labels.contains(label))
        {
            offset = (int)labels[label] - (int)next_opcode_pos;
            if (offset < -128 || offset > 127)
                throw Assembler_error("Displacement too big");
        }
        else
        {
            if (!future_labels.contains(label))
                future_labels[label] = vector<Address>();
            future_labels[label].push_back(offset_pos);
        }
        return offset;
    }
    
    void And(Imm imm)
    {
        dc_b(0x29);
        dc_b(imm.value());
    }
    
    void bne(const string& label)
    {
        dc_b(0xd0);
        dc_b(offset_to(label));
    }
    
    void bra(const string& label)
    {
        dc_b(0x80);
        dc_b(offset_to(label));
    }
    void cli()
    {
        dc_b(0x58);
    }
    void jsr(Abs addr)
    {
        dc_b(0x20);
        dc_w(addr.value());
    }    
    void lda(Imm imm)
    {
        dc_b(0xa9);
        dc_b(imm.value());
    }
    void pha()
    {
        dc_b(0x48);
    }
    void pla()
    {
        dc_b(0x68);
    }
    
    void rti()
    {
        dc_b(0x40);
    }
    
    void sta(Abs addr)
    {
        dc_b(0x8d);
        dc_w(addr.value());
    }
    void stz(const Abs& addr)
    {
        dc_b(0x9c);
        dc_w(addr.value());
    }
    

private:
    Address pc;
    Mem& mem;
    unordered_map<string, vector<Address>> future_labels;
    unordered_map<string, Address> labels;
};

}

#endif /* w65c02_assembler_hpp */
