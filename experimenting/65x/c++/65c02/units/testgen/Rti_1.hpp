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


#ifndef w65c02_Rti_1_h
#define w65c02_Rti_1_h
#include "../testgen.hpp"

namespace w65c02
{
class Rti_1: public Test
{
public:
    Rti_1(Mem& mem): Test(mem)
    {
        as.php();
        as.lda(Imm(0));
        as.pha();
        as.plp();
        
        as.ldx(Imm(0));
        as.bra("repeat");
        Abs handler(as.program_counter());
        as.rti();
        
        as.label("repeat");
        
        int ret_addr = as.program_counter() + 10;
        as.lda(Imm(ret_addr >> 8));
        as.pha();
        as.lda(Imm(ret_addr)); 
        as.pha();
        as.phx(); // rti should restore the status register with this value
        as.jmp(handler);
        if (ret_addr != as.program_counter())
            throw Assembler_error("Unexpected program size");
        
        as.php();
        as.pla();
        as.sta(Absx(0x3000));
        
        as.inx();
        as.bne("repeat");
        
        as.plp();
        as.rts();
        end();
    }
private:
    Address result_start()
    {
        return 0x3000;
    }
    
    size_t result_size()
    {
        return 256;
    }
};
}


#endif /* w65c02_Rti_1_h */
