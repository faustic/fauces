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

public class Jmp_abs_1 extends Test
{
    public Jmp_abs_1(Mem mem)
    {
        super(mem);
        as.php();
        as.lda(Imm.val(0));
        as.pha();
        as.plp();
        
        as.jmp(Abs.addr(0x2100));
        int[] xx = {0x4B, 0x40, 0x89, 0xB8, 0x06, 0x04, 0x47, 0xFD, 0xBD, 0x5C,
                                            0xAE, 0x99, 0x56, 0x5F, 0x18, 0xBF};
        for (int i = 0; i < 16; ++i)
        {
            as.lda(Imm.val(xx[i]));
            as.sta(Abs.addr(0x3001 + i));
        }    

        as.plp();
        as.rts();
        
        as.advance_to(0x2100);
        int[] x = {0x3C, 0x53, 0x0E, 0x9B, 0x3D, 0x28, 0x4B, 0xBA, 0xAC, 0x90,
                                            0x38, 0xCB, 0x3B, 0x2C, 0x5E, 0xF9};
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
        return 17;
    }
}
