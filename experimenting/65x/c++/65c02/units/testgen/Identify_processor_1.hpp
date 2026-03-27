//
/*
Licensed under the MIT License.
 
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


#ifndef w65c02_Identify_processor_1_h
#define w65c02_Identify_processor_1_h
#include "../testgen.hpp"

namespace w65c02
{
class Identify_processor_1: public Test
{
public:
    Identify_processor_1(Mem& mem): Test(mem)
    {
		
		string id6502 = "6502_";
		string id65c02 = "65c02";
		string id65816 = "65816";
		string done = "done";

        as.lda(Imm(0x36));
        as.sta(Abs(result_start()));
        as.lda(Imm(0x35));
        as.sta(Abs(result_start() + 1));

        as.sed();
		as.lda(Imm(0x99));
		as.clc();
		as.adc(Imm(1));
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
private:
    void save_processor_id(const string& id)
    {
        for (int i = 2, j = result_start() + 2;
             i < id.size() && i < 5;
             ++i, ++j)
        {
            as.lda(Imm(id[i]));
            as.sta(Abs(j));
        }
    }

    Address result_start()
    {
        return 0x3000;
	}
    
    size_t result_size()
    {
        return 5;
    }
};
}


#endif /* w65c02_Identify_processor_1_h */
