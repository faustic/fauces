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


#ifndef w65c02_Jmp_absxi_1_h
#define w65c02_Jmp_absxi_1_h
#include "../testgen.hpp"

namespace w65c02
{
class Jmp_absxi_1: public Test
{
public:
    Jmp_absxi_1(Mem& mem): Test(mem)
    {
        as.php();
        as.lda(Imm(0));
        as.pha();
        as.plp();
        
        as.lda(Imm(0x25));
        as.sta(Abs(0x3011));
        as.lda(Imm(0x21));
        as.sta(Abs(0x3012));
        as.ldx(Imm(1));
        as.jmp(Absxi(0x3010));
        int xx[] = { 0xB7, 0xE0, 0xD0, 0x6E, 0x63, 0x91, 0x47, 0xD0, 0x8E, 0x88,
                                            0x48, 0xBD, 0xBD, 0xE3, 0x6D, 0x92};
        for (int i = 0; i < 16; ++i)
        {
            as.lda(Imm(xx[i]));
            as.sta(Abs(0x3001 + i));
        }
        

        as.plp();
        as.rts();

        as.advance_to(0x2125);
        int x[] = {0xB9, 0xF5, 0xAC, 0xD2, 0xA3, 0x61, 0x47, 0x7A, 0xB7, 0xCB,
                                            0x73, 0x22, 0xFB, 0xAC, 0x1D, 0xDC};
        as.php();
        as.pla();
        as.sta(Abs(0x3000));
        for (int i = 0; i < 16; ++i)
        {
            as.lda(Imm(x[i]));
            as.sta(Abs(0x3001 + i));
        }
        
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
        return 19;
    }
};
}


#endif /* w65c02_Jmp_absxi_1_h */
