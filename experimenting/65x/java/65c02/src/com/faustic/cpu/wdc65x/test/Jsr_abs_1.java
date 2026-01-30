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

public class Jsr_abs_1 extends Test
{
    public Jsr_abs_1(Mem mem)
    {
        super(mem);
        as.php();
        as.lda(Imm.val(0));
        as.pha();
        as.plp();
        
        as.jsr(Abs.addr(0x2100));
        int[] xx = {0xF8, 0x14, 0xE7, 0x52, 0x6A, 0xF1, 0x46, 0x8A, 0x8E, 0xC7,
                                            0xEE, 0x0D, 0x6A, 0xEA, 0x95, 0x07};
        for (int i = 0; i < 16; ++i)
        {
            as.lda(Imm.val(xx[i]));
            as.sta(Abs.addr(0x3011 + i));
        }    

        as.plp();
        as.rts();
        
        as.advance_to(0x2100);
        int[] x = {0xB1, 0x9C, 0x0B, 0x77, 0x34, 0x23, 0x4F, 0xBA, 0xA3, 0x76,
                                            0x4B, 0x54, 0xA0, 0xD6, 0x67, 0x54};
        as.php();
        as.pla();
        as.sta(Abs.addr(0x3000));
        for (int i = 0; i < 16; ++i)
        {
            as.lda(Imm.val(x[i]));
            as.sta(Abs.addr(0x3001 + i));
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
        return 33;
    }
}
