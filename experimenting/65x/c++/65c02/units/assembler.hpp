// 65C02 code generation
//
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

class A
{
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

class Absi
{
public:
    Absi(Address value):
    priv_value(value)
    {}
    Address value() const
    {
        return priv_value;
    }
private:
    Address priv_value;
};

class Absx
{
public:
    Absx(Address value):
    priv_value(value)
    {}
    Address value() const
    {
        return priv_value;
    }
private:
    Address priv_value;
};

class Absxi
{
public:
    Absxi(Address value):
    priv_value(value)
    {}
    Address value() const
    {
        return priv_value;
    }
private:
    Address priv_value;
};

class Absy
{
public:
    Absy(Address value):
    priv_value(value)
    {}
    Address value() const
    {
        return priv_value;
    }
private:
    Address priv_value;
};

class Dp
{
public:
    Dp(Byte value):
    priv_value(value)
    {}
    Byte value() const
    {
        return priv_value;
    }
private:
    Byte priv_value;
};

class Dpi
{
public:
    Dpi(Byte value):
    priv_value(value)
    {}
    Byte value() const
    {
        return priv_value;
    }
private:
    Byte priv_value;
};

class Dpx
{
public:
    Dpx(Byte value):
    priv_value(value)
    {}
    Byte value() const
    {
        return priv_value;
    }
private:
    Byte priv_value;
};

class Dpxi
{
public:
    Dpxi(Byte value):
    priv_value(value)
    {}
    Byte value() const
    {
        return priv_value;
    }
private:
    Byte priv_value;
};

class Dpy
{
public:
    Dpy(Byte value):
    priv_value(value)
    {}
    Byte value() const
    {
        return priv_value;
    }
private:
    Byte priv_value;
};

class Dpiy
{
public:
    Dpiy(Byte value):
    priv_value(value)
    {}
    Byte value() const
    {
        return priv_value;
    }
private:
    Byte priv_value;
};

class Imm
{
public:
    Imm(Byte value):
    priv_value(value)
    {
        priv_value = value;
    }
    
    Byte value() const
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

    void advance_to(Address pc)
    {
        this->pc &= 0xffff;
        pc &= 0xffff;
        if (pc < this->pc)
            throw Assembler_error("Advancing backwards");
        this->pc = pc;
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
    
    Address label_position(const string& label)
    {
        if (!labels.contains(label))
            throw new Assembler_error("Undefined label " + label);
        return labels[label];
    }
    
    void adc(const Abs& addr)
    {
        dc_b(0x6d);
        dc_w(addr.value());
    }

    void adc(const Dp& addr)
    {
        dc_b(0x65);
        dc_b(addr.value());
    }
    
    void adc(const Dpi& addr)
    {
        dc_b(0x72);
        dc_b(addr.value());
    }
    
    void adc(const Absx& addr)
    {
        dc_b(0x7d);
        dc_w(addr.value());
    }
    
    void adc(const Absy& addr)
    {
        dc_b(0x79);
        dc_w(addr.value());
    }
    
    void adc(const Dpx& addr)
    {
        dc_b(0x75);
        dc_b(addr.value());
    }
    
    void adc(const Dpxi& addr)
    {
        dc_b(0x61);
        dc_b(addr.value());
    }
        
    void adc(const Dpiy& addr)
    {
        dc_b(0x71);
        dc_b(addr.value());
    }
        
    void adc(const Imm& imm)
    {
        dc_b(0x69);
        dc_b(imm.value());
    }
    
    void And(const Abs& addr)
    {
        dc_b(0x2d);
        dc_w(addr.value());
    }
    
    void And(const Dp& addr)
    {
        dc_b(0x25);
        dc_b(addr.value());
    }
    
    void And(const Dpi& addr)
    {
        dc_b(0x32);
        dc_b(addr.value());
    }
    
    void And(const Absx& addr)
    {
        dc_b(0x3d);
        dc_w(addr.value());
    }
    
    void And(const Absy& addr)
    {
        dc_b(0x39);
        dc_w(addr.value());
    }
    
    void And(const Dpx& addr)
    {
        dc_b(0x35);
        dc_b(addr.value());
    }
    
    void And(const Dpxi& addr)
    {
        dc_b(0x21);
        dc_b(addr.value());
    }
        
    void And(const Dpiy& addr)
    {
        dc_b(0x31);
        dc_b(addr.value());
    }
    
    void And(const Imm& imm)
    {
        dc_b(0x29);
        dc_b(imm.value());
    }
    
    void asl(A a)
    {
        dc_b(0x0a);
    }
        
    void asl(const Abs& addr)
    {
        dc_b(0x0e);
        dc_w(addr.value());
    }
    
    void asl(const Absx& addr)
    {
        dc_b(0x1e);
        dc_w(addr.value());
    }
    
    void asl(const Dp& addr)
    {
        dc_b(0x06);
        dc_b(addr.value());
    }
    
    void asl(const Dpx& addr)
    {
        dc_b(0x16);
        dc_b(addr.value());
    }
    
    void bbr(int bit, const Dp& addr, const string& label)
    {
        // It seems this instruction was not in the original WDC 65C02. It was
        // in Rockwell's implementation. But, apparently, WDC also added it
        // eventually.
        dc_b(((bit & 7) << 4) | 0xf);
        dc_b(addr.value());
        dc_b(offset_to(label));
    }
    
    void bbs(int bit, const Dp& addr, const string& label)
    {
        // It seems this instruction was not in the original WDC 65C02. It was
        // in Rockwell's implementation. But, apparently, WDC also added it
        // eventually.
        dc_b(((bit & 7) << 4) | 0x8f);
        dc_b(addr.value());
        dc_b(offset_to(label));
    }
    
    void bcc(const string& label)
    {
        dc_b(0x90);
        dc_b(offset_to(label));
    }
    
    void bcs(const string& label)
    {
        dc_b(0xb0);
        dc_b(offset_to(label));
    }
    
    void beq(const string& label)
    {
        dc_b(0xf0);
        dc_b(offset_to(label));
    }
    
    void bit(const Abs& addr)
    {
        dc_b(0x2c);
        dc_w(addr.value());
    }
    
    void bit(const Absx& addr)
    {
        dc_b(0x3c);
        dc_w(addr.value());
    }
    
    void bit(const Dp& addr)
    {
        dc_b(0x24);
        dc_b(addr.value());
    }
    
    void bit(const Dpx& addr)
    {
        dc_b(0x34);
        dc_b(addr.value());
    }
    
    void bit(const Imm& imm)
    {
        dc_b(0x89);
        dc_b(imm.value());
    }
    
    void bmi(const string& label)
    {
        dc_b(0x30);
        dc_b(offset_to(label));
    }
    
    void bne(const string& label)
    {
        dc_b(0xd0);
        dc_b(offset_to(label));
    }
    
    void bpl(const string& label)
    {
        dc_b(0x10);
        dc_b(offset_to(label));
    }
    
    void bra(const string& label)
    {
        dc_b(0x80);
        dc_b(offset_to(label));
    }

    void brk()
    {
        brk(Imm(0));
    }
    
    void brk(const Imm& imm)
    {
        dc_b(0);
        dc_b(imm.value());
    }
    
    void bvc(const string& label)
    {
        dc_b(0x50);
        dc_b(offset_to(label));
    }
    
    void bvs(const string& label)
    {
        dc_b(0x70);
        dc_b(offset_to(label));
    }
    
    void clc()
    {
        dc_b(0x18);
    }

    void cld()
    {
        dc_b(0xd8);
    }

    void cli()
    {
        dc_b(0x58);
    }

    void clv()
    {
        dc_b(0xb8);
    }
    
    void cmp(const Abs& addr)
    {
        dc_b(0xcd);
        dc_w(addr.value());
    }
    
    void cmp(const Absx& addr)
    {
        dc_b(0xdd);
        dc_w(addr.value());
    }
    
    void cmp(const Absy& addr)
    {
        dc_b(0xd9);
        dc_w(addr.value());
    }
    
    void cmp(const Dp& addr)
    {
        dc_b(0xc5);
        dc_b(addr.value());
    }
    
    void cmp(const Dpi& addr)
    {
        dc_b(0xd2);
        dc_b(addr.value());
    }
    
    void cmp(const Dpx& addr)
    {
        dc_b(0xd5);
        dc_b(addr.value());
    }
    
    void cmp(const Dpxi& addr)
    {
        dc_b(0xc1);
        dc_b(addr.value());
    }
        
    void cmp(const Dpiy& addr)
    {
        dc_b(0xd1);
        dc_b(addr.value());
    }
    
    void cmp(const Imm& imm)
    {
        dc_b(0xc9);
        dc_b(imm.value());
    }
    
    void cpx(const Abs& addr)
    {
        dc_b(0xec);
        dc_w(addr.value());
    }
    
    void cpx(const Dp& addr)
    {
        dc_b(0xe4);
        dc_b(addr.value());
    }
    
    void cpx(const Imm& imm)
    {
        dc_b(0xe0);
        dc_b(imm.value());
    }
        
    void cpy(const Abs& addr)
    {
        dc_b(0xcc);
        dc_w(addr.value());
    }
    
    void cpy(const Dp& addr)
    {
        dc_b(0xc4);
        dc_b(addr.value());
    }
    
    void cpy(const Imm& imm)
    {
        dc_b(0xc0);
        dc_b(imm.value());
    }
        
    void dec(A a)
    {
        dc_b(0x3a);
    }
        
    void dec(const Abs& addr)
    {
        dc_b(0xce);
        dc_w(addr.value());
    }
    
    void dec(const Dp& addr)
    {
        dc_b(0xc6);
        dc_b(addr.value());
    }
    
    void dec(const Absx& addr)
    {
        dc_b(0xde);
        dc_w(addr.value());
    }
    
    void dec(const Dpx& addr)
    {
        dc_b(0xd6);
        dc_b(addr.value());
    }
    
    void dex()
    {
        dc_b(0xca);
    }
    
    void dey()
    {
        dc_b(0x88);
    }
    
    void eor(const Abs& addr)
    {
        dc_b(0x4d);
        dc_w(addr.value());
    }
    
    void eor(const Absx& addr)
    {
        dc_b(0x5d);
        dc_w(addr.value());
    }
    
    void eor(const Absy& addr)
    {
        dc_b(0x59);
        dc_w(addr.value());
    }
    
    void eor(const Dp& addr)
    {
        dc_b(0x45);
        dc_b(addr.value());
    }
    
    void eor(const Dpi& addr)
    {
        dc_b(0x52);
        dc_b(addr.value());
    }
    
    void eor(const Dpx& addr)
    {
        dc_b(0x55);
        dc_b(addr.value());
    }
    
    void eor(const Dpxi& addr)
    {
        dc_b(0x41);
        dc_b(addr.value());
    }
        
    void eor(const Dpiy& addr)
    {
        dc_b(0x51);
        dc_b(addr.value());
    }
    
    void eor(const Imm& imm)
    {
        dc_b(0x49);
        dc_b(imm.value());
    }
    
    void inc(A a)
    {
        dc_b(0x1a);
    }
        
    void inc(const Abs& addr)
    {
        dc_b(0xee);
        dc_w(addr.value());
    }
    
    void inc(const Dp& addr)
    {
        dc_b(0xe6);
        dc_b(addr.value());
    }
    
    void inc(const Absx& addr)
    {
        dc_b(0xfe);
        dc_w(addr.value());
    }
    
    void inc(const Dpx& addr)
    {
        dc_b(0xf6);
        dc_b(addr.value());
    }
    
    void inx()
    {
        dc_b(0xe8);
    }

    void iny()
    {
        dc_b(0xc8);
    }
    
    void jmp(const Abs& addr)
    {
        dc_b(0x4c);
        dc_w(addr.value());
    }
    
    void jmp(Absi addr)
    {
        dc_b(0x6c);
        dc_w(addr.value());
    }
    
    void jmp(Absxi addr)
    {
        dc_b(0x7c);
        dc_w(addr.value());
    }
    
    void jsr(const Abs& addr)
    {
        dc_b(0x20);
        dc_w(addr.value());
    }    

    void lda(const Abs& addr)
    {
        dc_b(0xad);
        dc_w(addr.value());
    }
    
    void lda(const Absx& addr)
    {
        dc_b(0xbd);
        dc_w(addr.value());
    }
    
    void lda(const Absy& addr)
    {
        dc_b(0xb9);
        dc_w(addr.value());
    }
    
    void lda(const Dp& addr)
    {
        dc_b(0xa5);
        dc_b(addr.value());
    }
    
    void lda(const Dpi& addr)
    {
        dc_b(0xb2);
        dc_b(addr.value());
    }
    
    void lda(const Dpx& addr)
    {
        dc_b(0xb5);
        dc_b(addr.value());
    }
    
    void lda(const Dpxi& addr)
    {
        dc_b(0xa1);
        dc_b(addr.value());
    }
        
    void lda(const Dpiy& addr)
    {
        dc_b(0xb1);
        dc_b(addr.value());
    }
    
    void lda(const Imm& imm)
    {
        dc_b(0xa9);
        dc_b(imm.value());
    }

    void ldx(const Abs& addr)
    {
        dc_b(0xae);
        dc_w(addr.value());
    }
    
    void ldx(const Absy& addr)
    {
        dc_b(0xbe);
        dc_w(addr.value());
    }
    
    void ldx(const Dp& addr)
    {
        dc_b(0xa6);
        dc_b(addr.value());
    }
    
    void ldx(Dpy addr)
    {
        dc_b(0xb6);
        dc_b(addr.value());
    }
    
    void ldx(const Imm& imm)
    {
        dc_b(0xa2);
        dc_b(imm.value());
    }

    void ldy(const Abs& addr)
    {
        dc_b(0xac);
        dc_w(addr.value());
    }
    
    void ldy(const Absx& addr)
    {
        dc_b(0xbc);
        dc_w(addr.value());
    }
    
    void ldy(const Dp& addr)
    {
        dc_b(0xa4);
        dc_b(addr.value());
    }
    
    void ldy(const Dpx& addr)
    {
        dc_b(0xb4);
        dc_b(addr.value());
    }
    
    void ldy(const Imm& imm)
    {
        dc_b(0xa0);
        dc_b(imm.value());
    }
    
    void lsr(A a)
    {
        dc_b(0x4a);
    }
        
    void lsr(const Abs& addr)
    {
        dc_b(0x4e);
        dc_w(addr.value());
    }
    
    void lsr(const Absx& addr)
    {
        dc_b(0x5e);
        dc_w(addr.value());
    }
    
    void lsr(const Dp& addr)
    {
        dc_b(0x46);
        dc_b(addr.value());
    }
    
    void lsr(const Dpx& addr)
    {
        dc_b(0x56);
        dc_b(addr.value());
    }
    
    void nop()
    {
        dc_b(0xea);
    }
    
    void ora(const Abs& addr)
    {
        dc_b(0x0d);
        dc_w(addr.value());
    }
    
    void ora(const Absx& addr)
    {
        dc_b(0x1d);
        dc_w(addr.value());
    }
    
    void ora(const Absy& addr)
    {
        dc_b(0x19);
        dc_w(addr.value());
    }
    
    void ora(const Dp& addr)
    {
        dc_b(0x05);
        dc_b(addr.value());
    }
    
    void ora(const Dpi& addr)
    {
        dc_b(0x12);
        dc_b(addr.value());
    }
    
    void ora(const Dpx& addr)
    {
        dc_b(0x15);
        dc_b(addr.value());
    }
    
    void ora(const Dpxi& addr)
    {
        dc_b(0x01);
        dc_b(addr.value());
    }
        
    void ora(const Dpiy& addr)
    {
        dc_b(0x11);
        dc_b(addr.value());
    }
    
    void ora(const Imm& imm)
    {
        dc_b(0x09);
        dc_b(imm.value());
    }
    
    void pha()
    {
        dc_b(0x48);
    }

    void php()
    {
        dc_b(0x08);
    }

    void phx()
    {
        dc_b(0xda);
    }

    void phy()
    {
        dc_b(0x5a);
    }
    
    void pla()
    {
        dc_b(0x68);
    }
    
    void plp()
    {
        dc_b(0x28);
    }
    
    void plx()
    {
        dc_b(0xfa);
    }
    
    void ply()
    {
        dc_b(0x7a);
    }
    
    void rmb(int bit, const Dp& addr)
    {
        // It seems this instruction was not in the original WDC 65C02. It was
        // in Rockwell's implementation. But, apparently, WDC also added it
        // eventually.
        dc_b(((bit & 7) << 4) | 7);
        dc_b(addr.value());
    }
    
    void rol(A a)
    {
        dc_b(0x2a);
    }
        
    void rol(const Abs& addr)
    {
        dc_b(0x2e);
        dc_w(addr.value());
    }
    
    void rol(const Absx& addr)
    {
        dc_b(0x3e);
        dc_w(addr.value());
    }
    
    void rol(const Dp& addr)
    {
        dc_b(0x26);
        dc_b(addr.value());
    }
    
    void rol(const Dpx& addr)
    {
        dc_b(0x36);
        dc_b(addr.value());
    }
    
    void ror(A a)
    {
        dc_b(0x6a);
    }
        
    void ror(const Abs& addr)
    {
        dc_b(0x6e);
        dc_w(addr.value());
    }
    
    void ror(const Absx& addr)
    {
        dc_b(0x7e);
        dc_w(addr.value());
    }
    
    void ror(const Dp& addr)
    {
        dc_b(0x66);
        dc_b(addr.value());
    }
    
    void ror(const Dpx& addr)
    {
        dc_b(0x76);
        dc_b(addr.value());
    }
    
    void rti()
    {
        dc_b(0x40);
    }
    
    void rts()
    {
        dc_b(0x60);
    }
    
    void sbc(const Abs& addr)
    {
        dc_b(0xed);
        dc_w(addr.value());
    }
    
    void sbc(const Absx& addr)
    {
        dc_b(0xfd);
        dc_w(addr.value());
    }
    
    void sbc(const Absy& addr)
    {
        dc_b(0xf9);
        dc_w(addr.value());
    }
    
    void sbc(const Dp& addr)
    {
        dc_b(0xe5);
        dc_b(addr.value());
    }
    
    void sbc(const Dpi& addr)
    {
        dc_b(0xf2);
        dc_b(addr.value());
    }
    
    void sbc(const Dpx& addr)
    {
        dc_b(0xf5);
        dc_b(addr.value());
    }
    
    void sbc(const Dpxi& addr)
    {
        dc_b(0xe1);
        dc_b(addr.value());
    }
        
    void sbc(const Dpiy& addr)
    {
        dc_b(0xf1);
        dc_b(addr.value());
    }
    
    void sbc(const Imm& imm)
    {
        dc_b(0xe9);
        dc_b(imm.value());
    }
    
    void sec()
    {
        dc_b(0x38);
    }
    
    void sed()
    {
        dc_b(0xf8);
    }
    
    void sei()
    {
        dc_b(0x78);
    }
    
    void smb(int bit, const Dp& addr)
    {
        // It seems this instruction was not in the original WDC 65C02. It was
        // in Rockwell's implementation. But, apparently, WDC also added it
        // eventually.
        dc_b(((bit & 7) << 4) | 0x87);
        dc_b(addr.value());
    }
    
    void sta(const Abs& addr)
    {
        dc_b(0x8d);
        dc_w(addr.value());
    }

    void sta(const Absx& addr)
    {
        dc_b(0x9d);
        dc_w(addr.value());
    }

    void sta(const Absy& addr)
    {
        dc_b(0x99);
        dc_w(addr.value());
    }
    
    void sta(const Dp& addr)
    {
        dc_b(0x85);
        dc_b(addr.value());
    }
    
    void sta(const Dpi& addr)
    {
        dc_b(0x92);
        dc_b(addr.value());
    }
    
    void sta(const Dpx& addr)
    {
        dc_b(0x95);
        dc_b(addr.value());
    }
    
    void sta(const Dpxi& addr)
    {
        dc_b(0x81);
        dc_b(addr.value());
    }
        
    void sta(const Dpiy& addr)
    {
        dc_b(0x91);
        dc_b(addr.value());
    }
    
    void stp()
    {
        dc_b(0xdb);
    }
    
    void stx(const Abs& addr)
    {
        dc_b(0x8e);
        dc_w(addr.value());
    }
    
    void stx(const Dp& addr)
    {
        dc_b(0x86);
        dc_b(addr.value());
    }
    
    void stx(Dpy addr)
    {
        dc_b(0x96);
        dc_b(addr.value());
    }
    
    void sty(const Abs& addr)
    {
        dc_b(0x8c);
        dc_w(addr.value());
    }
    
    void sty(const Dp& addr)
    {
        dc_b(0x84);
        dc_b(addr.value());
    }
    
    void sty(const Dpx& addr)
    {
        dc_b(0x94);
        dc_b(addr.value());
    }
    
    void stz(const Abs& addr)
    {
        dc_b(0x9c);
        dc_w(addr.value());
    }
    
    void stz(const Absx& addr)
    {
        dc_b(0x9e);
        dc_w(addr.value());
    }
        
    void stz(const Dp& addr)
    {
        dc_b(0x64);
        dc_b(addr.value());
    }
    
    void stz(const Dpx& addr)
    {
        dc_b(0x74);
        dc_b(addr.value());
    }
    
    void tax()
    {
        dc_b(0xaa);
    }
    
    void tay()
    {
        dc_b(0xa8);
    }
    
    void trb(const Abs& addr)
    {
        dc_b(0x1c);
        dc_w(addr.value());
    }
    
    void trb(const Dp& addr)
    {
        dc_b(0x14);
        dc_b(addr.value());
    }
    
    void tsb(const Abs& addr)
    {
        dc_b(0x0c);
        dc_w(addr.value());
    }
    
    void tsb(const Dp& addr)
    {
        dc_b(0x04);
        dc_b(addr.value());
    }
    
    void tsx()
    {
        dc_b(0xba);
    }
    
    void txa()
    {
        dc_b(0x8a);
    }
    
    void txs()
    {
        dc_b(0x9a);
    }
    
    void tya()
    {
        dc_b(0x98);
    }
    
    void wai()
    {
        dc_b(0xcb);
    }
    
    void xce()
    {
        dc_b(0xfb);
    }
    

private:
    Address pc;
    Mem& mem;
    unordered_map<string, vector<Address>> future_labels;
    unordered_map<string, Address> labels;
};

}

#endif /* w65c02_assembler_hpp */
