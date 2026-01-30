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

package com.faustic.cpu.wdc65x.test;

import com.faustic.cpu.wdc65x.Assembler.*;
import com.faustic.memory.Mem;

public class Adc_dp_x_1 extends Test
{
    public Adc_dp_x_1(Mem mem)
    {
        super(mem);
        as.clc();
        as.cld();
        for (int i = 192; i < 224; ++i)
        {
            as.lda(Abs.addr(0x80 - 192 + i));
            as.sta(Abs.addr(0x7000 - 192 + i));
            as.lda(Imm.val(i));
            as.sta(Abs.addr(0x80 - 192 + i));
        }
        as.ldx(Imm.val(0));
        for (int i = 0; i < 32; ++i)
        {
            String part = "part_";
            as.label(part + i);
            as.phx();
            as.pla();
            as.phx();
            as.ldx(Imm.val(i));
            as.adc(Dp_x.addr(0x80));
            as.plx();
            as.sta(Abs_x.addr(0x3000 + i * 256));
            as.php();
            as.pla();
            as.and(Imm.val(0xc3));
            as.sta(Abs_x.addr(0x5000 + i * 256));
            as.inx();
            as.bne(part + i);
        }
        for (int i = 64; i < 96; ++i)
        {
            as.lda(Abs.addr(0x7000 - 64 + i));
            as.sta(Abs.addr(0x80 - 64 + i));
        }
        as.rts();
        end();
    }
    
    @Override
    public int result_start()
    {
        return 0x3000;
    }

    @Override
    public int result_size()
    {
        return 16384;
    }
}
