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

public class Jmp_abs___1 extends Test
{
    public Jmp_abs___1(Mem mem)
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
        as.jmp(Abs__.addr(0x3011));
        int[] xx = {0xD7, 0x9C, 0xD5, 0xF0, 0x57, 0x4C, 0x42, 0x1A, 0x93, 0xFE,
                                            0xDD, 0x92, 0x5C, 0x66, 0xEF, 0x65};
        for (int i = 0; i < 16; ++i)
        {
            as.lda(Imm.val(xx[i]));
            as.sta(Abs.addr(0x3001 + i));
        }
        

        as.plp();
        as.rts();

        as.advance_to(0x2125);
        int[] x = {0xC0, 0x1C, 0xAE, 0x67, 0x8D, 0x13, 0x43, 0x29, 0x8F, 0x7C,
                                            0x2E, 0xF8, 0xF3, 0xFF, 0x78, 0xA6};
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
