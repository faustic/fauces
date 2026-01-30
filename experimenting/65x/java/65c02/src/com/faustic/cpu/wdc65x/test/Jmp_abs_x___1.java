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

public class Jmp_abs_x___1 extends Test
{
    public Jmp_abs_x___1(Mem mem)
    {
        super(mem);
        as.php();
        as.lda(Imm.val(0));
        as.pha();
        as.plp();
        
        as.lda(Imm.val(0x25));
        as.sta(Abs.addr(0x3011));
        as.lda(Imm.val(0x21));
        as.sta(Abs.addr(0x3012));
        as.ldx(Imm.val(1));
        as.jmp(Abs_x__.addr(0x3010));
        int[] xx = { 0xB7, 0xE0, 0xD0, 0x6E, 0x63, 0x91, 0x47, 0xD0, 0x8E, 0x88,
                                            0x48, 0xBD, 0xBD, 0xE3, 0x6D, 0x92};
        for (int i = 0; i < 16; ++i)
        {
            as.lda(Imm.val(xx[i]));
            as.sta(Abs.addr(0x3001 + i));
        }
        

        as.plp();
        as.rts();

        as.advance_to(0x2125);
        int[] x = {0xB9, 0xF5, 0xAC, 0xD2, 0xA3, 0x61, 0x47, 0x7A, 0xB7, 0xCB,
                                            0x73, 0x22, 0xFB, 0xAC, 0x1D, 0xDC};
        as.php();
        as.pla();
        as.sta(Abs.addr(0x3000));
        for (int i = 0; i < 16; ++i)
        {
            as.lda(Imm.val(x[i]));
            as.sta(Abs.addr(0x3001 + i));
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
        return 19;
    }
}
