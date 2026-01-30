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

public class Tsx_1 extends Test
{
    public Tsx_1(Mem mem)
    {
        super(mem);
        
        as.php();
        as.lda(Imm.val(0));
        as.pha();
        as.plp();
        as.lda(Dp.addr(0xe0));
        as.sta(Abs.addr(0x7000));
                
        int[] check =
        {
            0x83, 0x44, 0x68, 0xEB, 0xC2, 0x60, 0x4C, 0x0F,
            0x8E, 0x87, 0x3D, 0xBB, 0x8C, 0x63, 0x5E, 0xEB
        };
        
        // Make sure the stack pointer is greater than 0x87
        as.tsx();
        as.stx(Dp.addr(0xe0));
        as.lda(Imm.val(0x87));
        as.cmp(Dp.addr(0xe0));
        as.bcc("position_stack");
        as.brk(); // Break if stack pointer is too small

        // Move stack pointer to 0x87
        as.ldy(Imm.val(0));
        as.label("position_stack");
        as.phy(); // Just to decrement the stack (pushed value will be unused)
        as.tsx();
        as.cpx(Imm.val(0x87));
        as.beq("stack_ok");
        as.bra("position_stack");
        
        // Save check data
        as.label("stack_ok");
        // Now S == 0x87
        for (int i = 0; i < check.length; ++i)
        {
            as.lda(Imm.val(check[i]));
            as.pha();
        }
        // Now S == 0x77
        
        // Test
        as.label("test");
        as.tsx();
        as.php();
        as.pla();
        as.sta(Abs_x.addr(0x3010 - 0x77)); // Save status from TSX operation
        as.lda(Abs_x.addr(0x100 + 1));
        as.sta(Abs_x.addr(0x3000 - 0x77));
        as.ply(); // Just to increment the stack (Y will be unused)
        as.cpx(Imm.val(0x87 - 1));
        as.bne("test");
        
        // Move stack pointer to original position
        as.label("restore_stack");
        as.pla();
        as.tsx();
        as.cpx(Dp.addr(0xe0));
        as.bne("restore_stack");
        
        
        as.lda(Abs.addr(0x7000));
        as.sta(Dp.addr(0xe0));
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
