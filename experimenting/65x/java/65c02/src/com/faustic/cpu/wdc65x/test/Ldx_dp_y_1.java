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

public class Ldx_dp_y_1 extends Test
{
    public Ldx_dp_y_1(Mem mem)
    {
        super(mem);
        as.php();
        as.lda(Imm.val(0));
        as.pha();
        as.plp();

        for (int i = 192; i < 224; ++i)
        {
            as.lda(Abs.addr(0x80 - 192 + i));
            as.sta(Abs.addr(0x7000 - 192 + i));
            as.lda(Imm.val(i));
            as.sta(Abs.addr(0x80 - 192 + i));
        }
        as.ldy(Imm.val(0));
        for (int i = 0; i < 32; ++i)
        {
            String part = "part_";
            as.label(part + i);
            as.phy();
            as.pla();
            as.phy();
            as.ldy(Imm.val(i));
            as.ldx(Dp_y.addr(0x80));
            as.ply();
            as.phx();
            as.pla();
            as.sta(Abs_y.addr(0x3000 + i * 256));
            as.php();
            as.pla();
            as.sta(Abs_y.addr(0x5000 + i * 256));
            as.iny();
            as.bne(part + i);
        }
        for (int i = 64; i < 96; ++i)
        {
            as.lda(Abs.addr(0x7000 - 64 + i));
            as.sta(Abs.addr(0x80 - 64 + i));
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
        return 16384;
    }
}
