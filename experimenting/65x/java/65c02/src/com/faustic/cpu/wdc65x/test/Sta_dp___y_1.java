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

public class Sta_dp___y_1 extends Test
{
    public Sta_dp___y_1(Mem mem)
    {
        super(mem);
        as.php();
        
        int[] check =
        {
            0x11, 0x82, 0x88, 0x11, 0xB5, 0xAF, 0x42, 0xA8,
            0xB5, 0x09, 0x4D, 0x9D, 0xB9, 0x23, 0x93, 0x69
        };
        
        int addr = 0x3000;
        as.lda(Dp.addr(0));
        as.sta(Abs.addr(0x7000));
        as.lda(Dp.addr(1));
        as.sta(Abs.addr(0x7001));
        as.lda(Imm.val(addr));
        as.sta(Dp.addr(0));
        as.lda(Imm.val(addr >> 8));
        as.sta(Dp.addr(1));
        
        for (int i = 0; i < check.length; ++i)
        {
            as.ldy(Imm.val(i));
            as.lda(Imm.val(check[i]));
            as.sta(Dp___y.addr(0));
        }
        
        as.lda(Abs.addr(0x7000));
        as.sta(Dp.addr(0));
        as.lda(Abs.addr(0x7001));
        as.sta(Dp.addr(1));


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
