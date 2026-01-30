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

public class Sta_dp_x___1 extends Test
{
    public Sta_dp_x___1(Mem mem)
    {
        super(mem);
        as.php();
        
        int[] check =
        {
            0x81, 0x6A, 0x46, 0x0C, 0x87, 0x0D, 0x48, 0x2D,
            0xB2, 0xF0, 0x90, 0x8D, 0xF8, 0x30, 0x60, 0x82
        };
        
        for (int i = 0; i < check.length; ++i)
        {
            as.lda(Dp.addr(i * 2));
            as.sta(Abs.addr(0x7000 + i * 2));
            as.lda(Dp.addr(i * 2 + 1));
            as.sta(Abs.addr(0x7000 + i * 2 + 1));
            int addr = 0x3000 + i;
            as.lda(Imm.val(addr));
            as.sta(Dp.addr(i * 2));
            as.lda(Imm.val(addr >> 8));
            as.sta(Dp.addr(i * 2 + 1));
        }
        
        for (int i = 0; i < check.length; ++i)
        {
            as.ldx(Imm.val(i * 2));
            as.lda(Imm.val(check[i]));
            as.sta(Dp_x__.addr(0));
        }
        
        for (int i = 0; i < check.length; ++i)
        {
            as.lda(Abs.addr(0x7000 + i * 2));
            as.sta(Dp.addr(i * 2));
            as.lda(Abs.addr(0x7000 + i * 2 + 1));
            as.sta(Dp.addr(i * 2 + 1));
        }


        as.plp();
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
        return 16;
    }
}
