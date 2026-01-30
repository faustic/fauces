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

public class Bbr_1 extends Test
{
    public Bbr_1(Mem mem)
    {
        super(mem);
        as.php();
        as.lda(Imm.val(0));
        as.pha();
        as.plp();
        
        as.lda(Dp.addr(0xea));
        as.sta(Abs.addr(0x7000));
        
        for (int i= 0; i < 8; ++i)
        {
            String loop = "begin_" + i;
            String check = "check_" + i;
            String met = "met_" + i;
            String end = "end" + i;
            int met_code = 0x78;
            int notmet_code = 0x69;
            as.ldx(Imm.val(0));
            as.label(loop);
            as.stx(Dp.addr(0xea));
            as.label(check);
            int nop = 0xea; // If BBR is not implemented, the 65c02 will
                            // interpret the zero page address as an
                            // instruction, so we make it the same as the opcode
                            // for NOP, to make the behaviour predictable.
            as.bbr(i, Dp.addr(nop), met);
            as.nop();
            as.nop();
            as.lda(Imm.val(notmet_code));
            as.sta(Abs_x.addr(0x3000 + i * 256));
            as.inx();
            as.bne(loop);
            as.bra(end);
            
            int bbr_num_bytes = 3;
            int alt_nop = 0x3b; // If BBR is not implemented, the 65c02 will
                                // interpret the offset as an instruction, so we
                                // make it the same as an unimplemented
                                // instruction, that acts like a NOP in 65c02
                                // processors (we cannot use the real NOP opcode
                                // because the offset would be negative).
            
            as.advance_to(as.label_position(check) + bbr_num_bytes + alt_nop);
            as.label(met);
            as.lda(Imm.val(met_code));
            as.sta(Abs_x.addr(0x3000 + i * 256));
            as.inx();
            as.bne(loop);
            
            as.label(end);
            
        }        
        
        as.lda(Abs.addr(0x7000));
        as.sta(Dp.addr(0xea));

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
        return 2048;
    }
}
