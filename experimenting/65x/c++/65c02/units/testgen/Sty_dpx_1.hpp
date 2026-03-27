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


#ifndef w65c02_Sty_dpx_1_h
#define w65c02_Sty_dpx_1_h
#include "../testgen.hpp"

namespace w65c02
{
class Sty_dpx_1: public Test
{
public:
    Sty_dpx_1(Mem& mem): Test(mem)
    {
        as.php();
        
        int check[] =
        {
            0xE6, 0x9F, 0x46, 0x43, 0xAC, 0x7B, 0x4F, 0xC1,
            0x82, 0xDE, 0x30, 0x38, 0x3F, 0xAA, 0xA4, 0x55
        };
        
        for (int i = 0; i < 16; ++i)
        {
            as.lda(Dp(i));
            as.sta(Abs(0x7000 + i));
        }
        
        for (int i = 0; i < 16; ++i)
        {
            as.ldy(Imm(check[i]));
            as.ldx(Imm(i));
            as.sty(Dpx(0));
            as.lda(Dpx(0));
            as.sta(Abs(0x3000 + i));
        }
        
        for (int i = 0; i < 16; ++i)
        {
            as.lda(Abs(0x7000 + i));
            as.sta(Dp(i));
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
        return 16;
    }
};
}


#endif /* w65c02_Sty_dpx_1_h */
