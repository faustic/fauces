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

public class Stz_dp_x_1 extends Test
{
    public Stz_dp_x_1(Mem mem)
    {
        super(mem);
        as.php();
        
        int[] check =
        {
            0xDE, 0x38, 0x57, 0xBE, 0x9C, 0xB4, 0x40, 0xA4, 
            0x88, 0xF9, 0x71, 0xFD, 0xA6, 0x54, 0x27, 0xE4,
            0x29, 0x5F, 0xFA, 0xB6, 0x1A, 0x25, 0x4D, 0x63,
            0xBD, 0x5F, 0x5A, 0x0C, 0xFC, 0xE9, 0xC2, 0xEA
        };
        
        for (int i = 0; i < check.length; ++i)
        {
            as.lda(Dp.addr(i));
            as.sta(Abs.addr(0x7000 + i));
        }
        
        for (int i = 0; i < check.length; ++i)
        {
            as.lda(Imm.val(check[i]));
            as.ldx(Imm.val(i));
            as.sta(Dp_x.addr(0));
            if (i % 2 != 0)
                as.stz(Dp_x.addr(0));
            as.lda(Dp_x.addr(0));
            as.sta(Abs.addr(0x3000 + i));
        }
        
        for (int i = 0; i < check.length; ++i)
        {
            as.lda(Abs.addr(0x7000 + i));
            as.sta(Dp.addr(i));
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
        return 32;
    }
}
