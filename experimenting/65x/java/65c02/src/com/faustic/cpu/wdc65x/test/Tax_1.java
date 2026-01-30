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

public class Tax_1 extends Test
{
    public Tax_1(Mem mem)
    {
        super(mem);
        as.php();
        as.lda(Imm.val(0));
        as.pha();
        as.plp();
        
        int[] check =
        {
            0x2C, 0x7C, 0x0C, 0x5F, 0xDF, 0x0E, 0x4A, 0xD2,
            0x88, 0x12, 0x22, 0x6B, 0x12, 0x66, 0xA2, 0x16
        };
        
        for (int i = 0; i < check.length; ++i)
        {
            as.lda(Imm.val(check[i]));
            as.ldx(Imm.val(0));
            as.tax();
            as.php();
            as.pla();
            as.stx(Abs.addr(0x3000 + i));
            as.sta(Abs.addr(0x3000 + check.length + i));
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
