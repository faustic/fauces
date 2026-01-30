/*
Licensed under the MIT License

Copyright (c) 2025 Faustic Inferno SL

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

public class Identify_processor_1 extends Test
{
	public Identify_processor_1(Mem mem)
	{
		super(mem);
		String id6502 = "6502_";
		String id65c02 = "65c02";
		String id65816 = "65816";
		String done = "done";

        as.lda(Imm.val(0x36));
        as.sta(Abs.addr(result_start()));
        as.lda(Imm.val(0x35));
        as.sta(Abs.addr(result_start() + 1));

        as.sed();
		as.lda(Imm.val(0x99));
		as.clc();
		as.adc(Imm.val(1));
		as.bmi(id6502); // The 6502 does not clear the sign flag in decimal mode
		
		as.clc();
		as.xce();
		as.bcc(id65c02); // The 65c02 treats XCE as a no-op
		as.xce();
		
		save_processor_id(id65816);
		as.bra(done);		
		
		as.label(id65c02);
		save_processor_id(id65c02);
		as.bra(done);

		as.label(id6502);
		save_processor_id(id6502);

		as.label(done);
		as.cld();
		as.rts();
		end();
	}
	
	private void save_processor_id(String id)
	{
		for (int i = 2, j = result_start() + 2; i < id.length() && i < 5; ++i, ++j)
		{
	        as.lda(Imm.val(id.codePointAt(i)));
	        as.sta(Abs.addr(j));
		}
	}

	@Override
    public int result_start()
    {
        return 0x3000;
    }

    @Override
    public int result_size()
    {
        return 5;
    }
    
}
