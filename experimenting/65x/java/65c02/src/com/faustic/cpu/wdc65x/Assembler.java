/*
Licensed under the MIT License

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

package com.faustic.cpu.wdc65x;

import java.util.HashMap;

import com.faustic.memory.*;

import java.util.ArrayList;

public class Assembler
{    
    static public class A
    {
        public static A reg()
        {
            return new A();
        }
    }
    
    static public class Imm
    {
        private int value;
        
        public Imm(int value)
        {
            this.value = value;
        }
        
        public static Imm val(int value)
        {
            return new Imm(value);
        }
        
        public int value()
        {
            return value;
        }
    }
    
    static public class Abs
    {
        private int value;
        
        public Abs(int value)
        {
            this.value = value;
        }
        
        public static Abs addr(int value)
        {
            return new Abs(value);
        }
        
        public int value()
        {
            return value;
        }
    }
    
    static public class Abs__
    {
        private int value;
        
        public Abs__(int value)
        {
            this.value = value;
        }
        
        public static Abs__ addr(int value)
        {
            return new Abs__(value);
        }
        
        public int value()
        {
            return value;
        }
    }
    
    static public class Abs_x__
    {
        private int value;
        
        public Abs_x__(int value)
        {
            this.value = value;
        }
        
        public static Abs_x__ addr(int value)
        {
            return new Abs_x__(value);
        }
        
        public int value()
        {
            return value;
        }
    }
    
    static public class Dp
    {
        private int value;
        
        public Dp(int value)
        {
            this.value = value;
        }
        
        public static Dp addr(int value)
        {
            return new Dp(value);
        }
        
        public int value()
        {
            return value;
        }
    }
    
    static public class Dp__
    {
        private int value;
        
        public Dp__(int value)
        {
            this.value = value;
        }
        
        public static Dp__ addr(int value)
        {
            return new Dp__(value);
        }
        
        public int value()
        {
            return value;
        }
    }
    
    static public class Abs_x
    {
        private int value;
        
        public Abs_x(int value)
        {
            this.value = value;
        }
        
        public static Abs_x addr(int value)
        {
            return new Abs_x(value);
        }
        
        public int value()
        {
            return value;
        }
    }
        
    static public class Abs_y
    {
        private int value;
        
        public Abs_y(int value)
        {
            this.value = value;
        }
        
        public static Abs_y addr(int value)
        {
            return new Abs_y(value);
        }
        
        public int value()
        {
            return value;
        }
    }
        
    static public class Dp_x
    {
        private int value;
        
        public Dp_x(int value)
        {
            this.value = value;
        }
        
        public static Dp_x addr(int value)
        {
            return new Dp_x(value);
        }
        
        public int value()
        {
            return value;
        }
    }
    
    static public class Dp_x__
    {
        private int value;
        
        public Dp_x__(int value)
        {
            this.value = value;
        }
        
        public static Dp_x__ addr(int value)
        {
            return new Dp_x__(value);
        }
        
        public int value()
        {
            return value;
        }
    }
    
    static public class Dp_y
    {
        private int value;
        
        public Dp_y(int value)
        {
            this.value = value;
        }
        
        public static Dp_y addr(int value)
        {
            return new Dp_y(value);
        }
        
        public int value()
        {
            return value;
        }
    }
    
    static public class Dp___y
    {
        private int value;
        
        public Dp___y(int value)
        {
            this.value = value;
        }
        
        public static Dp___y addr(int value)
        {
            return new Dp___y(value);
        }
        
        public int value()
        {
            return value;
        }
    }
    
    public Assembler(Mem mem, int start)
    {
        this.mem = mem;
        program_counter = start;
    }
    
    public int program_counter()
    {
        return program_counter;
    }
    
    public void advance_to(int pc)
    {
        program_counter &= 0xffff;
        pc &= 0xffff;
        if (pc < program_counter)
            throw new UnsupportedOperationException("Advancing backwards");
        program_counter = pc;
    }
    
    public void close()
    {
        if (future_labels.size() > 0)
            throw new UnsupportedOperationException("Undefined labels");
    }
    
    public void dc_b(int value)
    {
        mem.write8(program_counter++, value);
    }
        
    public void dc_w(int value)
    {
        mem.write16(program_counter, value);
        program_counter += 2;
    }
    
    public void label(String str)
    {
        // Associates a label to current program counter
        if (labels.containsKey(str))
            throw new UnsupportedOperationException("Duplicate label");
        if (future_labels.containsKey(str))
        {
            ArrayList<Integer> addresses = future_labels.get(str);
            for(int i = 0; i < addresses.size(); ++i)
            {
                int label_pos = program_counter;
                int offset_pos = addresses.get(i);
                int next_opcode_pos = offset_pos + 1;
                int offset = label_pos - next_opcode_pos;
                if (offset < -128 || offset > 127)
                    throw new UnsupportedOperationException
                                                    ("Displacement too big");
                mem.write8(offset_pos, offset);
            }
            future_labels.remove(str);
        }
        labels.put(str, program_counter);
    }
    
    public int label_position(String label)
    {
        if (!labels.containsKey(label))
            throw new UnsupportedOperationException("Undefined label " + label);
        return labels.get(label);
    }
    
    public int offset_to(String label)
    {
        // Calculates offset to next opcode from label position.
        // On entry, program_counter should point to offset parameter,
        // just before the next opcode.
        int offset_pos = program_counter;
        int next_opcode_pos = offset_pos + 1;
        int offset = 0;
        if (labels.containsKey(label))
        {
            offset = labels.get(label) - next_opcode_pos;
            if (offset < -128 || offset > 127)
                throw new UnsupportedOperationException("Displacement too big");
        }
        else
        {
            if (!future_labels.containsKey(label))
                future_labels.put(label, new ArrayList<Integer>());
            future_labels.get(label).add(offset_pos);
        }
        return offset;
    }
    
    public void adc(Abs addr)
    {
        dc_b(0x6d);
        dc_w(addr.value());
    }
    
    public void adc(Dp addr)
    {
        dc_b(0x65);
        dc_b(addr.value());
    }
    
    public void adc(Dp__ addr)
    {
        dc_b(0x72);
        dc_b(addr.value());
    }
    
    public void adc(Abs_x addr)
    {
        dc_b(0x7d);
        dc_w(addr.value());
    }
    
    public void adc(Abs_y addr)
    {
        dc_b(0x79);
        dc_w(addr.value());
    }
    
    public void adc(Dp_x addr)
    {
        dc_b(0x75);
        dc_b(addr.value());
    }
    
    public void adc(Dp_x__ addr)
    {
        dc_b(0x61);
        dc_b(addr.value());
    }
        
    public void adc(Dp___y addr)
    {
        dc_b(0x71);
        dc_b(addr.value());
    }
        
    public void adc(Imm imm)
    {
        dc_b(0x69);
        dc_b(imm.value());
    }
    
    public void and(Imm imm)
    {
        dc_b(0x29);
        dc_b(imm.value());
    }
    
    public void and(Abs addr)
    {
        dc_b(0x2d);
        dc_w(addr.value());
    }
    
    public void and(Dp addr)
    {
        dc_b(0x25);
        dc_b(addr.value());
    }
    
    public void and(Dp__ addr)
    {
        dc_b(0x32);
        dc_b(addr.value());
    }
    
    public void and(Abs_x addr)
    {
        dc_b(0x3d);
        dc_w(addr.value());
    }
    
    public void and(Abs_y addr)
    {
        dc_b(0x39);
        dc_w(addr.value());
    }
    
    public void and(Dp_x addr)
    {
        dc_b(0x35);
        dc_b(addr.value());
    }
    
    public void and(Dp_x__ addr)
    {
        dc_b(0x21);
        dc_b(addr.value());
    }
        
    public void and(Dp___y addr)
    {
        dc_b(0x31);
        dc_b(addr.value());
    }
    
    public void asl(A a)
    {
        dc_b(0x0a);
    }
        
    public void asl(Abs addr)
    {
        dc_b(0x0e);
        dc_w(addr.value());
    }
    
    public void asl(Abs_x addr)
    {
        dc_b(0x1e);
        dc_w(addr.value());
    }
    
    public void asl(Dp addr)
    {
        dc_b(0x06);
        dc_b(addr.value());
    }
    
    public void asl(Dp_x addr)
    {
        dc_b(0x16);
        dc_b(addr.value());
    }
    
    public void bbr(int bit, Dp addr, String label)
    {
        // It seems this instruction was not in the original WDC 65C02. It was
        // in Rockwell's implementation. But, apparently, WDC also added it
        // eventually.
        dc_b(((bit & 7) << 4) | 0xf);
        dc_b(addr.value()); 
        dc_b(offset_to(label));
    }
    
    public void bbs(int bit, Dp addr, String label)
    {
        // It seems this instruction was not in the original WDC 65C02. It was
        // in Rockwell's implementation. But, apparently, WDC also added it
        // eventually.
        dc_b(((bit & 7) << 4) | 0x8f);
        dc_b(addr.value()); 
        dc_b(offset_to(label));
    }
    
    public void bcc(String label)
    {
        dc_b(0x90);
        dc_b(offset_to(label));
    }
    
    public void bcs(String label)
    {
        dc_b(0xb0);
        dc_b(offset_to(label));
    }
    
    public void beq(String label)
    {
        dc_b(0xf0);
        dc_b(offset_to(label));
    }
    
    public void bit(Abs addr)
    {
        dc_b(0x2c);
        dc_w(addr.value());
    }
    
    public void bit(Abs_x addr)
    {
        dc_b(0x3c);
        dc_w(addr.value());
    }
    
    public void bit(Dp addr)
    {
        dc_b(0x24);
        dc_b(addr.value());
    }
    
    public void bit(Dp_x addr)
    {
        dc_b(0x34);
        dc_b(addr.value());
    }
    
    public void bit(Imm imm)
    {
        dc_b(0x89);
        dc_b(imm.value());
    }
    
    public void bmi(String label)
    {
        dc_b(0x30);
        dc_b(offset_to(label));
    }
    
    public void bne(String label)
    {
        dc_b(0xd0);
        dc_b(offset_to(label));
    }
    
    public void bpl(String label)
    {
        dc_b(0x10);
        dc_b(offset_to(label));
    }
    
    public void bra(String label)
    {
        dc_b(0x80);
        dc_b(offset_to(label));
    }
    
    public void brk()
    {
        brk(Imm.val(0));
    }
    
    public void brk(Imm imm)
    {
        dc_b(0);
        dc_b(imm.value());
    }
    
    public void bvc(String label)
    {
        dc_b(0x50);
        dc_b(offset_to(label));
    }
    
    public void bvs(String label)
    {
        dc_b(0x70);
        dc_b(offset_to(label));
    }
    
    public void clc()
    {
        dc_b(0x18);
    }
    
    public void cld()
    {
        dc_b(0xd8);
    }
    
    public void cli()
    {
        dc_b(0x58);
    }
    
    public void clv()
    {
        dc_b(0xb8);
    }
    
    public void cmp(Abs addr)
    {
        dc_b(0xcd);
        dc_w(addr.value());
    }
    
    public void cmp(Abs_x addr)
    {
        dc_b(0xdd);
        dc_w(addr.value());
    }
    
    public void cmp(Abs_y addr)
    {
        dc_b(0xd9);
        dc_w(addr.value());
    }
    
    public void cmp(Dp addr)
    {
        dc_b(0xc5);
        dc_b(addr.value());
    }
    
    public void cmp(Dp__ addr)
    {
        dc_b(0xd2);
        dc_b(addr.value());
    }
    
    public void cmp(Dp_x addr)
    {
        dc_b(0xd5);
        dc_b(addr.value());
    }
    
    public void cmp(Dp_x__ addr)
    {
        dc_b(0xc1);
        dc_b(addr.value());
    }
        
    public void cmp(Dp___y addr)
    {
        dc_b(0xd1);
        dc_b(addr.value());
    }
    
    public void cmp(Imm imm)
    {
        dc_b(0xc9);
        dc_b(imm.value());
    }
    
    public void cpx(Abs addr)
    {
        dc_b(0xec);
        dc_w(addr.value());
    }
    
    public void cpx(Dp addr)
    {
        dc_b(0xe4);
        dc_b(addr.value());
    }
    
    public void cpx(Imm imm)
    {
        dc_b(0xe0);
        dc_b(imm.value());
    }
        
    public void cpy(Abs addr)
    {
        dc_b(0xcc);
        dc_w(addr.value());
    }
    
    public void cpy(Dp addr)
    {
        dc_b(0xc4);
        dc_b(addr.value());
    }
    
    public void cpy(Imm imm)
    {
        dc_b(0xc0);
        dc_b(imm.value());
    }
        
    public void dec(A a)
    {
        dc_b(0x3a);
    }
        
    public void dec(Abs addr)
    {
        dc_b(0xce);
        dc_w(addr.value());
    }
    
    public void dec(Dp addr)
    {
        dc_b(0xc6);
        dc_b(addr.value());
    }
    
    public void dec(Abs_x addr)
    {
        dc_b(0xde);
        dc_w(addr.value());
    }
    
    public void dec(Dp_x addr)
    {
        dc_b(0xd6);
        dc_b(addr.value());
    }
    
    public void dex()
    {
        dc_b(0xca);
    }
    
    public void dey()
    {
        dc_b(0x88);
    }
    
    public void eor(Abs addr)
    {
        dc_b(0x4d);
        dc_w(addr.value());
    }
    
    public void eor(Abs_x addr)
    {
        dc_b(0x5d);
        dc_w(addr.value());
    }
    
    public void eor(Abs_y addr)
    {
        dc_b(0x59);
        dc_w(addr.value());
    }
    
    public void eor(Dp addr)
    {
        dc_b(0x45);
        dc_b(addr.value());
    }
    
    public void eor(Dp__ addr)
    {
        dc_b(0x52);
        dc_b(addr.value());
    }
    
    public void eor(Dp_x addr)
    {
        dc_b(0x55);
        dc_b(addr.value());
    }
    
    public void eor(Dp_x__ addr)
    {
        dc_b(0x41);
        dc_b(addr.value());
    }
        
    public void eor(Dp___y addr)
    {
        dc_b(0x51);
        dc_b(addr.value());
    }
    
    public void eor(Imm imm)
    {
        dc_b(0x49);
        dc_b(imm.value());
    }
    
    public void inc(A a)
    {
        dc_b(0x1a);
    }
        
    public void inc(Abs addr)
    {
        dc_b(0xee);
        dc_w(addr.value());
    }
    
    public void inc(Dp addr)
    {
        dc_b(0xe6);
        dc_b(addr.value());
    }
    
    public void inc(Abs_x addr)
    {
        dc_b(0xfe);
        dc_w(addr.value());
    }
    
    public void inc(Dp_x addr)
    {
        dc_b(0xf6);
        dc_b(addr.value());
    }
    
    public void inx()
    {
        dc_b(0xe8);
    }
    
    public void iny()
    {
        dc_b(0xc8);
    }
    
    public void jmp(Abs addr)
    {
        dc_b(0x4c);
        dc_w(addr.value());
    }
    
    public void jmp(Abs__ addr)
    {
        dc_b(0x6c);
        dc_w(addr.value());
    }
    
    public void jmp(Abs_x__ addr)
    {
        dc_b(0x7c);
        dc_w(addr.value());
    }
    
    public void jsr(Abs addr)
    {
        dc_b(0x20);
        dc_w(addr.value());
    }
    
    public void lda(Abs addr)
    {
        dc_b(0xad);
        dc_w(addr.value());
    }
    
    public void lda(Abs_x addr)
    {
        dc_b(0xbd);
        dc_w(addr.value());
    }
    
    public void lda(Abs_y addr)
    {
        dc_b(0xb9);
        dc_w(addr.value());
    }
    
    public void lda(Dp addr)
    {
        dc_b(0xa5);
        dc_b(addr.value());
    }
    
    public void lda(Dp__ addr)
    {
        dc_b(0xb2);
        dc_b(addr.value());
    }
    
    public void lda(Dp_x addr)
    {
        dc_b(0xb5);
        dc_b(addr.value());
    }
    
    public void lda(Dp_x__ addr)
    {
        dc_b(0xa1);
        dc_b(addr.value());
    }
        
    public void lda(Dp___y addr)
    {
        dc_b(0xb1);
        dc_b(addr.value());
    }
    
    public void lda(Imm imm)
    {
        dc_b(0xa9);
        dc_b(imm.value());
    }
    
    public void ldx(Abs addr)
    {
        dc_b(0xae);
        dc_w(addr.value());
    }
    
    public void ldx(Abs_y addr)
    {
        dc_b(0xbe);
        dc_w(addr.value());
    }
    
    public void ldx(Dp addr)
    {
        dc_b(0xa6);
        dc_b(addr.value());
    }
    
    public void ldx(Dp_y addr)
    {
        dc_b(0xb6);
        dc_b(addr.value());
    }
    
    public void ldx(Imm imm)
    {
        dc_b(0xa2);
        dc_b(imm.value());
    }
    
    public void ldy(Abs addr)
    {
        dc_b(0xac);
        dc_w(addr.value());
    }
    
    public void ldy(Abs_x addr)
    {
        dc_b(0xbc);
        dc_w(addr.value());
    }
    
    public void ldy(Dp addr)
    {
        dc_b(0xa4);
        dc_b(addr.value());
    }
    
    public void ldy(Dp_x addr)
    {
        dc_b(0xb4);
        dc_b(addr.value());
    }
    
    public void ldy(Imm imm)
    {
        dc_b(0xa0);
        dc_b(imm.value());
    }
    
    public void lsr(A a)
    {
        dc_b(0x4a);
    }
        
    public void lsr(Abs addr)
    {
        dc_b(0x4e);
        dc_w(addr.value());
    }
    
    public void lsr(Abs_x addr)
    {
        dc_b(0x5e);
        dc_w(addr.value());
    }
    
    public void lsr(Dp addr)
    {
        dc_b(0x46);
        dc_b(addr.value());
    }
    
    public void lsr(Dp_x addr)
    {
        dc_b(0x56);
        dc_b(addr.value());
    }
    
    public void nop()
    {
        dc_b(0xea);
    }
    
    public void ora(Abs addr)
    {
        dc_b(0x0d);
        dc_w(addr.value());
    }
    
    public void ora(Abs_x addr)
    {
        dc_b(0x1d);
        dc_w(addr.value());
    }
    
    public void ora(Abs_y addr)
    {
        dc_b(0x19);
        dc_w(addr.value());
    }
    
    public void ora(Dp addr)
    {
        dc_b(0x05);
        dc_b(addr.value());
    }
    
    public void ora(Dp__ addr)
    {
        dc_b(0x12);
        dc_b(addr.value());
    }
    
    public void ora(Dp_x addr)
    {
        dc_b(0x15);
        dc_b(addr.value());
    }
    
    public void ora(Dp_x__ addr)
    {
        dc_b(0x01);
        dc_b(addr.value());
    }
        
    public void ora(Dp___y addr)
    {
        dc_b(0x11);
        dc_b(addr.value());
    }
    
    public void ora(Imm imm)
    {
        dc_b(0x09);
        dc_b(imm.value());
    }
    
    public void pha()
    {
        dc_b(0x48);
    }
    
    public void php()
    {
        dc_b(8);
    }
    
    public void phx()
    {
        dc_b(0xda);
    }
    
    public void phy()
    {
        dc_b(0x5a);
    }
    
    public void pla()
    {
        dc_b(0x68);
    }
    
    public void plp()
    {
        dc_b(0x28);
    }
    
    public void plx()
    {
        dc_b(0xfa);
    }
    
    public void ply()
    {
        dc_b(0x7a);
    }
    
    public void rmb(int bit, Dp addr)
    {
        // It seems this instruction was not in the original WDC 65C02. It was
        // in Rockwell's implementation. But, apparently, WDC also added it
        // eventually.
        dc_b(((bit & 7) << 4) | 7);
        dc_b(addr.value());        
    }
    
    public void rol(A a)
    {
        dc_b(0x2a);
    }
        
    public void rol(Abs addr)
    {
        dc_b(0x2e);
        dc_w(addr.value());
    }
    
    public void rol(Abs_x addr)
    {
        dc_b(0x3e);
        dc_w(addr.value());
    }
    
    public void rol(Dp addr)
    {
        dc_b(0x26);
        dc_b(addr.value());
    }
    
    public void rol(Dp_x addr)
    {
        dc_b(0x36);
        dc_b(addr.value());
    }
    
    public void ror(A a)
    {
        dc_b(0x6a);
    }
        
    public void ror(Abs addr)
    {
        dc_b(0x6e);
        dc_w(addr.value());
    }
    
    public void ror(Abs_x addr)
    {
        dc_b(0x7e);
        dc_w(addr.value());
    }
    
    public void ror(Dp addr)
    {
        dc_b(0x66);
        dc_b(addr.value());
    }
    
    public void ror(Dp_x addr)
    {
        dc_b(0x76);
        dc_b(addr.value());
    }
    
    public void rti()
    {
        dc_b(0x40);
    }
    
    public void rts()
    {
        dc_b(0x60);
    }
    
    public void sbc(Abs addr)
    {
        dc_b(0xed);
        dc_w(addr.value());
    }
    
    public void sbc(Abs_x addr)
    {
        dc_b(0xfd);
        dc_w(addr.value());
    }
    
    public void sbc(Abs_y addr)
    {
        dc_b(0xf9);
        dc_w(addr.value());
    }
    
    public void sbc(Dp addr)
    {
        dc_b(0xe5);
        dc_b(addr.value());
    }
    
    public void sbc(Dp__ addr)
    {
        dc_b(0xf2);
        dc_b(addr.value());
    }
    
    public void sbc(Dp_x addr)
    {
        dc_b(0xf5);
        dc_b(addr.value());
    }
    
    public void sbc(Dp_x__ addr)
    {
        dc_b(0xe1);
        dc_b(addr.value());
    }
        
    public void sbc(Dp___y addr)
    {
        dc_b(0xf1);
        dc_b(addr.value());
    }
    
    public void sbc(Imm imm)
    {
        dc_b(0xe9);
        dc_b(imm.value());
    }
    
    public void sec()
    {
        dc_b(0x38);
    }
    
    public void sed()
    {
        dc_b(0xf8);
    }
    
    public void sei()
    {
        dc_b(0x78);
    }
    
    public void smb(int bit, Dp addr)
    {
        // It seems this instruction was not in the original WDC 65C02. It was
        // in Rockwell's implementation. But, apparently, WDC also added it
        // eventually.
        dc_b(((bit & 7) << 4) | 0x87);
        dc_b(addr.value());     
    }
    
    public void sta(Abs addr)
    {
        dc_b(0x8d);
        dc_w(addr.value());
    }
    
    public void sta(Abs_x addr)
    {
        dc_b(0x9d);
        dc_w(addr.value());
    }
    
    public void sta(Abs_y addr)
    {
        dc_b(0x99);
        dc_w(addr.value());
    }
    
    public void sta(Dp addr)
    {
        dc_b(0x85);
        dc_b(addr.value());
    }
    
    public void sta(Dp__ addr)
    {
        dc_b(0x92);
        dc_b(addr.value());
    }
    
    public void sta(Dp_x addr)
    {
        dc_b(0x95);
        dc_b(addr.value());
    }
    
    public void sta(Dp_x__ addr)
    {
        dc_b(0x81);
        dc_b(addr.value());
    }
        
    public void sta(Dp___y addr)
    {
        dc_b(0x91);
        dc_b(addr.value());
    }
    
    public void stp()
    {
        dc_b(0xdb);
    }
    
    public void stx(Abs addr)
    {
        dc_b(0x8e);
        dc_w(addr.value());
    }
    
    public void stx(Dp addr)
    {
        dc_b(0x86);
        dc_b(addr.value());
    }
    
    public void stx(Dp_y addr)
    {
        dc_b(0x96);
        dc_b(addr.value());
    }
    
    public void sty(Abs addr)
    {
        dc_b(0x8c);
        dc_w(addr.value());
    }
    
    public void sty(Dp addr)
    {
        dc_b(0x84);
        dc_b(addr.value());
    }
    
    public void sty(Dp_x addr)
    {
        dc_b(0x94);
        dc_b(addr.value());
    }
    
    public void stz(Abs addr)
    {
        dc_b(0x9c);
        dc_w(addr.value());
    }
        
    public void stz(Abs_x addr)
    {
        dc_b(0x9e);
        dc_w(addr.value());
    }
        
    public void stz(Dp addr)
    {
        dc_b(0x64);
        dc_b(addr.value());
    }
    
    public void stz(Dp_x addr)
    {
        dc_b(0x74);
        dc_b(addr.value());
    }
    
    public void tax()
    {
        dc_b(0xaa);
    }
    
    public void tay()
    {
        dc_b(0xa8);
    }
    
    public void trb(Abs addr)
    {
        dc_b(0x1c);
        dc_w(addr.value());
    }
    
    public void trb(Dp addr)
    {
        dc_b(0x14);
        dc_b(addr.value());
    }
    
    public void tsb(Abs addr)
    {
        dc_b(0x0c);
        dc_w(addr.value());
    }
    
    public void tsb(Dp addr)
    {
        dc_b(0x04);
        dc_b(addr.value());
    }
    
    public void tsx()
    {
        dc_b(0xba);
    }
    
    public void txa()
    {
        dc_b(0x8a);
    }
    
    public void txs()
    {
        dc_b(0x9a);
    }
    
    public void tya()
    {
        dc_b(0x98);
    }
    
    public void wai()
    {
        dc_b(0xcb);
    }
    
    public void xce()
    {
        dc_b(0xfb);
    }
    
    private Mem mem;
    
    private int program_counter;
    
    private HashMap<String, ArrayList<Integer>> future_labels =
                                    new HashMap<String, ArrayList<Integer>>();
    private HashMap<String, Integer> labels = new HashMap<String, Integer>();
}
