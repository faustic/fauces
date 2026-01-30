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

public class Stz_abs_x_1 extends Test
{
    public Stz_abs_x_1(Mem mem)
    {
        super(mem);
        as.php();
        
        int[] check =
        {
            0x9F, 0xE4, 0x51, 0x4E, 0x10, 0xCF, 0x4F, 0xA6,
            0x86, 0xA2, 0x4B, 0x35, 0x28, 0xED, 0x7B, 0x6F,
            0x8F, 0xF8, 0xEB, 0xD8, 0xC2, 0xEC, 0x47, 0xA3,
            0xB7, 0x5C, 0x41, 0x46, 0xB0, 0xBA, 0x95, 0xDA
        };
        
        for (int i = 0; i < check.length; ++i)
        {
            as.ldx(Imm.val(i));
            as.lda(Imm.val(check[i]));
            as.sta(Abs_x.addr(0x3000));
            if (i % 2 != 0)
                as.stz(Abs_x.addr(0x3000));
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
