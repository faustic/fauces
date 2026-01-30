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

import com.faustic.memory.*;


public class Disassembler implements Processor
{
    public Disassembler(Mem mem, int start, int max_count, boolean rockwell)
    {
        this.mem = mem;
        this.rockwell = rockwell;
        go(start, max_count);
    }
    
    public void go(int start_addr)
    {
        go(start_addr, max_count);
    }
    
    public void go(int start_addr, int max_count)
    {
        this.max_count = max_count;
        parser = new Parser(this, mem);
        parser.parse(start_addr);
        for (int count = 1; count < max_count; ++count)
            parser.parse();
    }
    
    public void flush()
    {
        buffer = "";
    }
    
    public void write_instruction_address(int addr)
    {
        buffer += String.format("$%04x: ", addr);
    }
    
    public void print()
    {
        System.out.println(text());
        flush();
    }
    
    public String text()
    {
        return buffer;
    }
    
    private int data_byte(int addr)
    {
        write_instruction_address(addr);
        buffer += "dc " + String.format("$%02x", mem.read8(addr)) + "\n";
        return addr + 1;
    }
    
   @Override
    public int adc_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "adc " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int adc_abs_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "adc " + String.format("$%04x,x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int adc_abs_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "adc " + String.format("$%04x,y", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int adc_abslong(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr++);
        if (rockwell)
        {
            int imm = (byte)mem.read8(addr++);
            buffer += "bbr6 " + String.format("$%02x", abs_addr) + "," +
                          String.format("$%04x", (addr + imm) & 0xffff) + "\n";
        }
        else
        {
            abs_addr += mem.read16(addr) << 8;
            addr += 2;
            buffer += "adc " + String.format("$%06x", abs_addr) + "\n";
        }
        return addr;
    }
    
    @Override
    public int adc_abslong_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr++);
        if (rockwell)
        {
            int imm = (byte)mem.read8(addr++);
            buffer += "bbr7 " + String.format("$%02x", abs_addr) + "," +
                          String.format("$%04x", (addr + imm) & 0xffff) + "\n";
        }
        else
        {
            abs_addr += mem.read16(addr) << 8;
            addr += 2;
            buffer += "adc " + String.format("$%06x,x", abs_addr) + "\n";
        }
        return addr;
    }
    
    @Override
    public int adc_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "adc " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int adc_dp__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "adc " + String.format("($%02x)", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int adc_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "adc " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int adc_dp_x__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "adc " + String.format("($%02x,x)", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int adc_dp___y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "adc " + String.format("($%02x),y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int adc_dp__long(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        if (rockwell)
            buffer += "rmb6 " + String.format("$%02x", abs_addr) + "\n";
        else
            buffer += "adc " + String.format("[$%02x]", abs_addr) + "\n";
        return addr + 1;
    }
    
    @Override
    public int adc_dp__long_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        if (rockwell)
            buffer += "rmb7 " + String.format("$%02x", abs_addr) + "\n";
        else
            buffer += "adc " + String.format("[$%02x],y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int adc_imm(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = mem.read8(addr);
        buffer += "adc " + String.format("#$%02x", imm) + "\n";
        return addr + 1;
    }

    @Override
    public int adc_sr(int addr)
    {
        write_instruction_address(addr - 1);
        int rel_addr = mem.read8(addr);
        buffer += "adc " + String.format("$%02x,s", rel_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int adc_sr__y(int addr)
    {
        write_instruction_address(addr - 1);
        int rel_addr = mem.read8(addr);
        buffer += "adc " + String.format("($%02x,s),y", rel_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int and_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "and " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int and_abs_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "and " + String.format("$%04x,x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int and_abs_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "and " + String.format("$%04x,y", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int and_abslong(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr++);
        if (rockwell)
        {
            int imm = (byte)mem.read8(addr++);
            buffer += "bbr2 " + String.format("$%02x", abs_addr) + "," +
                          String.format("$%04x", (addr + imm) & 0xffff) + "\n";
        }
        else
        {
            abs_addr += mem.read16(addr) << 8;
            addr += 2;
            buffer += "and " + String.format("$%06x", abs_addr) + "\n";
        }
        return addr;
    }
    
    @Override
    public int and_abslong_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr++);
        if (rockwell)
        {
            int imm = (byte)mem.read8(addr++);
            buffer += "bbr3 " + String.format("$%02x", abs_addr) + "," +
                          String.format("$%04x", (addr + imm) & 0xffff) + "\n";
        }
        else
        {
            abs_addr += mem.read16(addr) << 8;
            addr += 2;
            buffer += "and " + String.format("$%06x,x", abs_addr) + "\n";
        }
        return addr;
    }
    
    @Override
    public int and_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "and " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int and_dp__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "and " + String.format("($%02x)", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int and_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "and " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int and_dp_x__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "and " + String.format("($%02x,x)", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int and_dp___y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "and " + String.format("($%02x),y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int and_dp__long(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        if (rockwell)
            buffer += "rmb2 " + String.format("$%02x", abs_addr) + "\n";
        else
            buffer += "and " + String.format("[$%02x]", abs_addr) + "\n";
        return addr + 1;
    }
    
    @Override
    public int and_dp__long_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        if (rockwell)
            buffer += "rmb3 " + String.format("$%02x", abs_addr) + "\n";
        else
            buffer += "and " + String.format("[$%02x],y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int and_imm(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = mem.read8(addr);
        buffer += "and " + String.format("#$%02x", imm) + "\n";
        return addr + 1;
    }

    @Override
    public int and_sr(int addr)
    {
        write_instruction_address(addr - 1);
        int rel_addr = mem.read8(addr);
        buffer += "and " + String.format("$%02x,s", rel_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int and_sr__y(int addr)
    {
        write_instruction_address(addr - 1);
        int rel_addr = mem.read8(addr);
        buffer += "and " + String.format("($%02x,s),y", rel_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int asl_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "asl " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int asl_abs_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "asl " + String.format("$%04x,x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int asl_acc(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "asl a\n";
        return addr;
    }

    @Override
    public int asl_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "asl " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int asl_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "asl " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int bcc(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = (byte)mem.read8(addr);
        int abs_addr = (addr + imm + 1) & 0xffff;
        buffer += "bcc " + String.format("$%04x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int bcs(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = (byte)mem.read8(addr);
        int abs_addr = (addr + imm + 1) & 0xffff;
        buffer += "bcs " + String.format("$%04x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int beq(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = (byte)mem.read8(addr);
        int abs_addr = (addr + imm + 1) & 0xffff;
        buffer += "beq " + String.format("$%04x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int bit_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "bit " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int bit_abs_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "bit " + String.format("$%04x,x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int bit_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "bit " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int bit_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "bit " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int bit_imm(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = mem.read8(addr);
        buffer += "bit " + String.format("#$%02x", imm) + "\n";
        return addr + 1;
    }

    @Override
    public int bmi(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = (byte)mem.read8(addr);
        int abs_addr = (addr + imm + 1) & 0xffff;
        buffer += "bmi " + String.format("$%04x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int bne(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = (byte)mem.read8(addr);
        int abs_addr = (addr + imm + 1) & 0xffff;
        buffer += "bne " + String.format("$%04x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int bpl(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = (byte)mem.read8(addr);
        int abs_addr = (addr + imm + 1) & 0xffff;
        buffer += "bpl " + String.format("$%04x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int bra(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = (byte)mem.read8(addr);
        int abs_addr = (addr + imm + 1) & 0xffff;
        buffer += "bra " + String.format("$%04x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int brk(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "brk\n";
        return data_byte(addr);
    }

    @Override
    public int brl(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = mem.read16(addr);
        int abs_addr = ((addr + imm + 1) & 0xffff) | (addr & 0xff0000);
        buffer += "brl " + String.format("$%06x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int bvc(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = (byte)mem.read8(addr);
        int abs_addr = (addr + imm + 1) & 0xffff;
        buffer += "bvc " + String.format("$%04x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int bvs(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = (byte)mem.read8(addr);
        int abs_addr = (addr + imm + 1) & 0xffff;
        buffer += "bvs " + String.format("$%04x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int clc(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "clc" + "\n";
        return addr;
    }
    
    @Override
    public int cld(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "cld" + "\n";
        return addr;
    }
    
    @Override
    public int cli(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "cli" + "\n";
        return addr;
    }
    
    @Override
    public int clv(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "clv" + "\n";
        return addr;
    }
    
    @Override
    public int cmp_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "cmp " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int cmp_abs_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "cmp " + String.format("$%04x,x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int cmp_abs_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "cmp " + String.format("$%04x,y", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int cmp_abslong(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr++);
        if (rockwell)
        {
            int imm = (byte)mem.read8(addr++);
            buffer += "bbs4 " + String.format("$%02x", abs_addr) + "," +
                          String.format("$%04x", (addr + imm) & 0xffff) + "\n";
        }
        else
        {
            abs_addr += mem.read16(addr) << 8;
            addr += 2;
            buffer += "cmp " + String.format("$%06x", abs_addr) + "\n";
        }
        return addr;
    }
    
    @Override
    public int cmp_abslong_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr++);
        if (rockwell)
        {
            int imm = (byte)mem.read8(addr++);
            buffer += "bbs5 " + String.format("$%02x", abs_addr) + "," +
                          String.format("$%04x", (addr + imm) & 0xffff) + "\n";
        }
        else
        {
            abs_addr += mem.read16(addr) << 8;
            addr += 2;
            buffer += "cmp " + String.format("$%06x,x", abs_addr) + "\n";
        }
        return addr;
    }
    
    @Override
    public int cmp_dp__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "cmp " + String.format("($%02x)", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int cmp_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "cmp " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int cmp_dp_x__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "cmp " + String.format("($%02x,x)", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int cmp_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "cmp " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int cmp_dp___y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "cmp " + String.format("($%02x),y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int cmp_dp__long(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        if (rockwell)
            buffer += "smb4 " + String.format("$%02x", abs_addr) + "\n";
        else
            buffer += "cmp " + String.format("[$%02x]", abs_addr) + "\n";
        return addr + 1;
    }
    
    @Override
    public int cmp_dp__long_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        if (rockwell)
            buffer += "smb5 " + String.format("$%02x", abs_addr) + "\n";
        else
            buffer += "cmp " + String.format("[$%02x],y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int cmp_imm(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = mem.read8(addr);
        buffer += "cmp " + String.format("#$%02x", imm) + "\n";
        return addr + 1;
    }

    @Override
    public int cmp_sr(int addr)
    {
        write_instruction_address(addr - 1);
        int rel_addr = mem.read8(addr);
        buffer += "cmp " + String.format("$%02x,s", rel_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int cmp_sr__y(int addr)
    {
        write_instruction_address(addr - 1);
        int rel_addr = mem.read8(addr);
        buffer += "cmp " + String.format("($%02x,s),y", rel_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int cop(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "cop\n";
        return data_byte(addr);
    }

    @Override
    public int cpx_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "cpx " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int cpx_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "cpx " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int cpx_imm(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = mem.read8(addr);
        buffer += "cpx " + String.format("#$%02x", imm) + "\n";
        return addr + 1;
    }

    @Override
    public int cpy_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "cpy " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int cpy_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "cpy " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int cpy_imm(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = mem.read8(addr);
        buffer += "cpy " + String.format("#$%02x", imm) + "\n";
        return addr + 1;
    }

    @Override
    public int dec_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "dec " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int dec_abs_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "dec " + String.format("$%04x,x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int dec_acc(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "dec a\n";
        return addr;
    }

    @Override
    public int dec_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "dec " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int dec_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "dec " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int dex(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "dex\n";
        return addr;
    }

    @Override
    public int dey(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "dey\n";
        return addr;
    }

    @Override
    public int eor_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "eor " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int eor_abs_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "eor " + String.format("$%04x,x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int eor_abs_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "eor " + String.format("$%04x,y", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int eor_abslong(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr++);
        if (rockwell)
        {
            int imm = (byte)mem.read8(addr++);
            buffer += "bbr4 " + String.format("$%02x", abs_addr) + "," +
                          String.format("$%04x", (addr + imm) & 0xffff) + "\n";
        }
        else
        {
            abs_addr += mem.read16(addr) << 8;
            addr += 2;
            buffer += "eor " + String.format("$%06x", abs_addr) + "\n";
        }
        return addr;
    }
    
    @Override
    public int eor_abslong_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr++);
        if (rockwell)
        {
            int imm = (byte)mem.read8(addr++);
            buffer += "bbr5 " + String.format("$%02x", abs_addr) + "," +
                          String.format("$%04x", (addr + imm) & 0xffff) + "\n";
        }
        else
        {
            abs_addr += mem.read16(addr) << 8;
            addr += 2;
            buffer += "eor " + String.format("$%06x,x", abs_addr) + "\n";
        }
        return addr;
    }
    
    @Override
    public int eor_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "eor " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int eor_dp__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "eor " + String.format("($%02x)", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int eor_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "eor " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int eor_dp_x__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "eor " + String.format("($%02x,x)", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int eor_dp___y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "eor " + String.format("($%02x),y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int eor_dp__long(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        if (rockwell)
            buffer += "rmb4 " + String.format("$%02x", abs_addr) + "\n";
        else
            buffer += "eor " + String.format("[$%02x]", abs_addr) + "\n";
        return addr + 1;
    }
    
    @Override
    public int eor_dp__long_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        if (rockwell)
            buffer += "rmb5 " + String.format("$%02x", abs_addr) + "\n";
        else
            buffer += "eor " + String.format("[$%02x],y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int eor_imm(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = mem.read8(addr);
        buffer += "eor " + String.format("#$%02x", imm) + "\n";
        return addr + 1;
    }

    @Override
    public int eor_sr(int addr)
    {
        write_instruction_address(addr - 1);
        int rel_addr = mem.read8(addr);
        buffer += "eor " + String.format("$%02x,s", rel_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int eor_sr__y(int addr)
    {
        write_instruction_address(addr - 1);
        int rel_addr = mem.read8(addr);
        buffer += "eor " + String.format("($%02x,s),y", rel_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int inc_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "inc " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int inc_abs_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "inc " + String.format("$%04x,x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int inc_acc(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "inc a\n";
        return addr;
    }

    @Override
    public int inc_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "inc " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int inc_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "inc " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int inx(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "inx" + "\n";
        return addr;
    }
    
    @Override
    public int iny(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "iny" + "\n";
        return addr;
    }
    
    @Override
    public int jml(int addr)
    {
        write_instruction_address(addr - 1);
        int bank = mem.read8(addr);
        int abs_addr = (bank << 16) | mem.read16(addr);
        buffer += "jml " + String.format("$%06x", abs_addr) + "\n";
        return addr + 3;
    }

    @Override
    public int jml__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "jml " + String.format("[$%04x]", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int jmp_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "jmp " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int jmp_abs__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "jmp " + String.format("($%04x)", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int jmp_abs_x__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "jmp " + String.format("($%04x,x)", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int jsl(int addr)
    {
        write_instruction_address(addr - 1);
        int bank = mem.read8(addr);
        int abs_addr = (bank << 16) | mem.read16(addr);
        buffer += "jsl " + String.format("$%06x", abs_addr) + "\n";
        return addr + 3;
    }

   @Override
    public int jsr_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "jsr " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

   @Override
   public int jsr_abs_x__(int addr)
   {
       write_instruction_address(addr - 1);
       int abs_addr = mem.read16(addr);
       buffer += "jsr " + String.format("($%04x,x)", abs_addr) + "\n";
       return addr + 2;
   }

    @Override
    public int lda_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "lda " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int lda_abs_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "lda " + String.format("$%04x,x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int lda_abs_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "lda " + String.format("$%04x,y", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int lda_abslong(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr++);
        if (rockwell)
        {
            int imm = (byte)mem.read8(addr++);
            buffer += "bbs2 " + String.format("$%02x", abs_addr) + "," +
                          String.format("$%04x", (addr + imm) & 0xffff) + "\n";
        }
        else
        {
            abs_addr += mem.read16(addr) << 8;
            addr += 2;
            buffer += "lda " + String.format("$%06x", abs_addr) + "\n";
        }
        return addr;
    }
    
    @Override
    public int lda_abslong_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr++);
        if (rockwell)
        {
            int imm = (byte)mem.read8(addr++);
            buffer += "bbs3 " + String.format("$%02x", abs_addr) + "," +
                          String.format("$%04x", (addr + imm) & 0xffff) + "\n";
        }
        else
        {
            abs_addr += mem.read16(addr) << 8;
            addr += 2;
            buffer += "lda " + String.format("$%06x,x", abs_addr) + "\n";
        }
        return addr;
    }
    
    @Override
    public int lda_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "lda " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int lda_dp__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "lda " + String.format("($%02x)", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int lda_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "lda " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int lda_dp_x__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "lda " + String.format("($%02x,x)", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int lda_dp___y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "lda " + String.format("($%02x),y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int lda_dp__long(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        if (rockwell)
            buffer += "smb2 " + String.format("$%02x", abs_addr) + "\n";
        else
            buffer += "lda " + String.format("[$%02x]", abs_addr) + "\n";
        return addr + 1;
    }
    
    @Override
    public int lda_dp__long_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        if (rockwell)
            buffer += "smb3 " + String.format("$%02x", abs_addr) + "\n";
        else
            buffer += "lda " + String.format("[$%02x],y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int lda_imm(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = mem.read8(addr);
        buffer += "lda " + String.format("#$%02x", imm) + "\n";
        return addr + 1;
    }

    @Override
    public int lda_sr(int addr)
    {
        write_instruction_address(addr - 1);
        int rel_addr = mem.read8(addr);
        buffer += "lda " + String.format("$%02x,s", rel_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int lda_sr__y(int addr)
    {
        write_instruction_address(addr - 1);
        int rel_addr = mem.read8(addr);
        buffer += "lda " + String.format("($%02x,s),y", rel_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int ldx_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "ldx " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int ldx_abs_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "ldx " + String.format("$%04x,y", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int ldx_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "ldx " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int ldx_dp_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "ldx " + String.format("$%02x,y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int ldx_imm(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = mem.read8(addr);
        buffer += "ldx " + String.format("#$%02x", imm) + "\n";
        return addr + 1;
    }

    @Override
    public int ldy_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "ldy " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int ldy_abs_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "ldy " + String.format("$%04x,x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int ldy_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "ldy " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int ldy_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "ldy " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int ldy_imm(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = mem.read8(addr);
        buffer += "ldy " + String.format("#$%02x", imm) + "\n";
        return addr + 1;
    }

    @Override
    public int lsr_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "lsr " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int lsr_abs_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "lsr " + String.format("$%04x,x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int lsr_acc(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "lsr a\n";
        return addr;
    }

    @Override
    public int lsr_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "lsr " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int lsr_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "lsr " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int mvn(int addr)
    {
        write_instruction_address(addr - 1);
        int destbk = mem.read8(addr) << 16;
        int srcbk = mem.read8(addr + 1) << 16;
        buffer += "mvn " + String.format("$%06x,$%06x", srcbk, destbk) + "\n";
        return addr + 2;
    }

    @Override
    public int mvp(int addr)
    {
        write_instruction_address(addr - 1);
        int destbk = mem.read8(addr) << 16;
        int srcbk = mem.read8(addr + 1) << 16;
        buffer += "mvp " + String.format("$%06x,$%06x", srcbk, destbk) + "\n";
        return addr + 2;
    }

    @Override
    public int nop(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "nop" + "\n";
        return addr;
    }
    
    @Override
    public int ora_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "ora " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int ora_abs_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "ora " + String.format("$%04x,x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int ora_abs_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "ora " + String.format("$%04x,y", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int ora_abslong(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr++);
        if (rockwell)
        {
            int imm = (byte)mem.read8(addr++);
            buffer += "bbr0 " + String.format("$%02x", abs_addr) + "," +
                          String.format("$%04x", (addr + imm) & 0xffff) + "\n";
        }
        else
        {
            abs_addr += mem.read16(addr) << 8;
            addr += 2;
            buffer += "ora " + String.format("$%06x", abs_addr) + "\n";
        }
        return addr;
    }
    
    @Override
    public int ora_abslong_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr++);
        if (rockwell)
        {
            int imm = (byte)mem.read8(addr++);
            buffer += "bbr1 " + String.format("$%02x", abs_addr) + "," +
                          String.format("$%04x", (addr + imm) & 0xffff) + "\n";
        }
        else
        {
            abs_addr += mem.read16(addr) << 8;
            addr += 2;
            buffer += "ora " + String.format("$%06x,x", abs_addr) + "\n";
        }
        return addr;
    }
    
    @Override
    public int ora_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "ora " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int ora_dp__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "ora " + String.format("($%02x)", abs_addr) + "\n";
        return addr + 1;
    }
    
   @Override
    public int ora_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "ora " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int ora_dp_x__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "ora " + String.format("($%02x,x)", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int ora_dp___y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "ora " + String.format("($%02x),y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int ora_dp__long(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        if (rockwell)
            buffer += "rmb0 " + String.format("$%02x", abs_addr) + "\n";
        else
            buffer += "ora " + String.format("[$%02x]", abs_addr) + "\n";
        return addr + 1;
    }
    
    @Override
    public int ora_dp__long_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        if (rockwell)
            buffer += "rmb1 " + String.format("$%02x", abs_addr) + "\n";
        else
            buffer += "ora " + String.format("[$%02x],y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int ora_imm(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = mem.read8(addr);
        buffer += "ora " + String.format("#$%02x", imm) + "\n";
        return addr + 1;
    }

    @Override
    public int ora_sr(int addr)
    {
        write_instruction_address(addr - 1);
        int rel_addr = mem.read8(addr);
        buffer += "ora " + String.format("$%02x,s", rel_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int ora_sr__y(int addr)
    {
        write_instruction_address(addr - 1);
        int rel_addr = mem.read8(addr);
        buffer += "ora " + String.format("($%02x,s),y", rel_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int pea(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "pea " + String.format("$%04x", mem.read16(addr)) + "\n";
        return addr + 1;
    }

    @Override
    public int pei(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "pei " + String.format("($%02x)", mem.read8(addr)) + "\n";
        return addr + 1;
    }

    @Override
    public int per(int addr)
    {
        write_instruction_address(addr - 1);
        int ea = mem.read16(addr);
        addr += 2;
        ea += addr;
        buffer += "per " + String.format("($%04x)", ea) + "\n";
        return addr;
    }

    @Override
    public int pha(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "pha\n";
        return addr ;
    }
    
    @Override
    public int phb(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "phb\n";
        return addr ;
    }
    
    @Override
    public int phd(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "phd\n";
        return addr ;
    }
    
    @Override
    public int phk(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "phk\n";
        return addr ;
    }
    
    @Override
    public int php(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "php\n";
        return addr ;
    }
    
    @Override
    public int phx(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "phx\n";
        return addr ;
    }
    
    @Override
    public int phy(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "phy\n";
        return addr ;
    }
    
    @Override
    public int pla(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "pla\n";
        return addr ;
    }
    
    @Override
    public int plb(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "plb\n";
        return addr ;
    }
    
    @Override
    public int pld(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "pld\n";
        return addr ;
    }
    
    @Override
    public int plp(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "plp\n";
        return addr ;
    }
    
    @Override
    public int plx(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "plx\n";
        return addr ;
    }
    
    @Override
    public int ply(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "ply\n";
        return addr ;
    }
    
    @Override
    public int rep(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = mem.read8(addr);
        buffer += "rep " + String.format("#$%02x", imm) + "\n";
        return addr + 1;
    }

    @Override
    public int rol_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "rol " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int rol_abs_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "rol " + String.format("$%04x,x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int rol_acc(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "rol a\n";
        return addr;
    }

    @Override
    public int rol_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "rol " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int rol_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "rol " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int ror_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "ror " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int ror_abs_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "ror " + String.format("$%04x,x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int ror_acc(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "ror a\n";
        return addr;
    }

    @Override
    public int ror_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "ror " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int ror_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "ror " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int rti(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "rti\n";
        return addr;
    }
    
    @Override
    public int rtl(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "rts\n";
        return addr;
    }
    
    @Override
    public int rts(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "rts\n";
        return addr;
    }
    
    @Override
    public int sbc_abs(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "sbc " + String.format("$%04x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int sbc_abs_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "sbc " + String.format("$%04x,x", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int sbc_abs_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "sbc " + String.format("$%04x,y", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int sbc_abslong(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr++);
        if (rockwell)
        {
            int imm = (byte)mem.read8(addr++);
            buffer += "bbs6 " + String.format("$%02x", abs_addr) + "," +
                          String.format("$%04x", (addr + imm) & 0xffff) + "\n";
        }
        else
        {
            abs_addr += mem.read16(addr) << 8;
            addr += 2;
            buffer += "sbc " + String.format("$%06x", abs_addr) + "\n";
        }
        return addr;
    }
    
    @Override
    public int sbc_abslong_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr++);
        if (rockwell)
        {
            int imm = (byte)mem.read8(addr++);
            buffer += "bbs7 " + String.format("$%02x", abs_addr) + "," +
                          String.format("$%04x", (addr + imm) & 0xffff) + "\n";
        }
        else
        {
            abs_addr += mem.read16(addr) << 8;
            addr += 2;
            buffer += "sbc " + String.format("$%06x,x", abs_addr) + "\n";
        }
        return addr;
    }
    
    @Override
    public int sbc_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "sbc " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sbc_dp__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "sbc " + String.format("($%02x)", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sbc_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "sbc " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sbc_dp_x__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "sbc " + String.format("($%02x,x)", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sbc_dp___y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "sbc " + String.format("($%02x),y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sbc_dp__long(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        if (rockwell)
            buffer += "smb6 " + String.format("$%02x", abs_addr) + "\n";
        else
            buffer += "sbc " + String.format("[$%02x]", abs_addr) + "\n";
        return addr + 1;
    }
    
    @Override
    public int sbc_dp__long_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        if (rockwell)
            buffer += "smb7 " + String.format("$%02x", abs_addr) + "\n";
        else
            buffer += "sbc " + String.format("[$%02x],y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sbc_imm(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = mem.read8(addr);
        buffer += "sbc " + String.format("#$%02x", imm) + "\n";
        return addr + 1;
    }

    @Override
    public int sbc_sr(int addr)
    {
        write_instruction_address(addr - 1);
        int rel_addr = mem.read8(addr);
        buffer += "sbc " + String.format("$%02x,s", rel_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sbc_sr__y(int addr)
    {
        write_instruction_address(addr - 1);
        int rel_addr = mem.read8(addr);
        buffer += "sbc " + String.format("($%02x,s),y", rel_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sec(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "sec" + "\n";
        return addr;
    }
    
    @Override
    public int sed(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "sed" + "\n";
        return addr;
    }
    
    @Override
    public int sei(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "sei" + "\n";
        return addr;
    }
    
    @Override
    public int sep(int addr)
    {
        write_instruction_address(addr - 1);
        int imm = mem.read8(addr);
        buffer += "sep " + String.format("#$%02x", imm) + "\n";
        return addr + 1;
    }

    @Override
    public int sta_abs(int addr)
    {
        
        write_instruction_address(addr - 1);
        buffer += "sta " + String.format("$%04x", mem.read16(addr)) + "\n";
        return addr + 2;
    }
    
    @Override
    public int sta_abs_x(int addr)
    {
        
        write_instruction_address(addr - 1);
        buffer += "sta " + String.format("$%04x", mem.read16(addr)) + ",x\n";
        return addr + 2;
    }
    
    @Override
    public int sta_abs_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read16(addr);
        buffer += "sta " + String.format("$%04x,y", abs_addr) + "\n";
        return addr + 2;
    }

    @Override
    public int sta_abslong(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr++);
        if (rockwell)
        {
            int imm = (byte)mem.read8(addr++);
            buffer += "bbs0 " + String.format("$%02x", abs_addr) + "," +
                          String.format("$%04x", (addr + imm) & 0xffff) + "\n";
        }
        else
        {
            abs_addr += mem.read16(addr) << 8;
            addr += 2;
            buffer += "sta " + String.format("$%06x", abs_addr) + "\n";
        }
        return addr;
    }
    
    @Override
    public int sta_abslong_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr++);
        if (rockwell)
        {
            int imm = (byte)mem.read8(addr++);
            buffer += "bbs1 " + String.format("$%02x", abs_addr) + "," +
                          String.format("$%04x", (addr + imm) & 0xffff) + "\n";
        }
        else
        {
            abs_addr += mem.read16(addr) << 8;
            addr += 2;
            buffer += "sta " + String.format("$%06x,x", abs_addr) + "\n";
        }
        return addr;
    }
    
    @Override
    public int sta_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "sta " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sta_dp__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "sta " + String.format("($%02x)", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sta_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "sta " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sta_dp_x__(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "sta " + String.format("($%02x,x)", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sta_dp___y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "sta " + String.format("($%02x),y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sta_dp__long(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        if (rockwell)
            buffer += "smb0 " + String.format("$%02x", abs_addr) + "\n";
        else
            buffer += "sta " + String.format("[$%02x]", abs_addr) + "\n";
        return addr + 1;
    }
    
    @Override
    public int sta_dp__long_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        if (rockwell)
            buffer += "smb1 " + String.format("$%02x", abs_addr) + "\n";
        else
            buffer += "sta " + String.format("[$%02x],y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sta_sr(int addr)
    {
        write_instruction_address(addr - 1);
        int rel_addr = mem.read8(addr);
        buffer += "ora " + String.format("$%02x,s", rel_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sta_sr__y(int addr)
    {
        write_instruction_address(addr - 1);
        int rel_addr = mem.read8(addr);
        buffer += "adc " + String.format("($%02x,s),y", rel_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int stp(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "stp" + "\n";
        return addr;
    }
    
    @Override
    public int stx_abs(int addr)
    {
        
        write_instruction_address(addr - 1);
        buffer += "stx " + String.format("$%04x", mem.read16(addr)) + "\n";
        return addr + 2;
    }
    
    @Override
    public int stx_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "stx " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int stx_dp_y(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "stx " + String.format("$%02x,y", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sty_abs(int addr)
    {
        
        write_instruction_address(addr - 1);
        buffer += "sty " + String.format("$%04x", mem.read16(addr)) + "\n";
        return addr + 2;
    }
    
    @Override
    public int sty_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "sty " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int sty_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "sty " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int stz_abs(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "stz " + String.format("$%04x", mem.read16(addr)) + "\n";
        return addr + 2;
    }
    
    @Override
    public int stz_abs_x(int addr)
    {
        
        write_instruction_address(addr - 1);
        buffer += "stz " + String.format("$%04x", mem.read16(addr)) + ",x\n";
        return addr + 2;
    }
    
    @Override
    public int stz_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "stz " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int stz_dp_x(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "stz " + String.format("$%02x,x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int tax(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "tax" + "\n";
        return addr;
    }
    
    @Override
    public int tcd(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "tcd" + "\n";
        return addr;
    }
    
    @Override
    public int tcs(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "tcs" + "\n";
        return addr;
    }
    
    @Override
    public int tdc(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "tdc" + "\n";
        return addr;
    }
    
    @Override
    public int tay(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "tay" + "\n";
        return addr;
    }
    
    @Override
    public int trb_abs(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "trb " + String.format("$%04x", mem.read16(addr)) + "\n";
        return addr + 2;
    }
    
    @Override
    public int trb_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "trb " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int tsb_abs(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "tsb " + String.format("$%04x", mem.read16(addr)) + "\n";
        return addr + 2;
    }
    
    @Override
    public int tsb_dp(int addr)
    {
        write_instruction_address(addr - 1);
        int abs_addr = mem.read8(addr);
        buffer += "tsb " + String.format("$%02x", abs_addr) + "\n";
        return addr + 1;
    }

    @Override
    public int tsc(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "tsc" + "\n";
        return addr;
    }
    
    @Override
    public int tsx(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "tsx" + "\n";
        return addr;
    }
    
    @Override
    public int txa(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "txa" + "\n";
        return addr;
    }
    
    @Override
    public int txs(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "txs" + "\n";
        return addr;
    }
    
    @Override
    public int txy(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "txy" + "\n";
        return addr;
    }
    
    @Override
    public int tya(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "tya" + "\n";
        return addr;
    }
    
    @Override
    public int tyx(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "tyx" + "\n";
        return addr;
    }
    
    @Override
    public int wai(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "wai" + "\n";
        return addr;
    }
    
    @Override
    public int wdm(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "wdm\n";
        return data_byte(addr);
    }

    @Override
    public int xba(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "xba" + "\n";
        return addr;
    }
    
    @Override
    public int xce(int addr)
    {
        write_instruction_address(addr - 1);
        buffer += "xce" + "\n";
        return addr;
    }
    
    private Mem mem;
    private String buffer = "";    
    private Parser parser;
    private int max_count;
    private boolean rockwell = false;
}
