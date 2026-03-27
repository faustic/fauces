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


#ifndef w65c02_Stz_dpx_1_h
#define w65c02_Stz_dpx_1_h
#include "../testgen.hpp"

namespace w65c02
{
class Stz_dpx_1: public Test
{
public:
    Stz_dpx_1(Mem& mem): Test(mem)
    {
        as.php();
        
        int check[] =
        {
            0xDE, 0x38, 0x57, 0xBE, 0x9C, 0xB4, 0x40, 0xA4, 
            0x88, 0xF9, 0x71, 0xFD, 0xA6, 0x54, 0x27, 0xE4,
            0x29, 0x5F, 0xFA, 0xB6, 0x1A, 0x25, 0x4D, 0x63,
            0xBD, 0x5F, 0x5A, 0x0C, 0xFC, 0xE9, 0xC2, 0xEA
        };
        
        for (int i = 0; i < 32; ++i)
        {
            as.lda(Dp(i));
            as.sta(Abs(0x7000 + i));
        }
        
        for (int i = 0; i < 32; ++i)
        {
            as.lda(Imm(check[i]));
            as.ldx(Imm(i));
            as.sta(Dpx(0));
            if (i % 2 != 0)
                as.stz(Dpx(0));
            as.lda(Dpx(0));
            as.sta(Abs(0x3000 + i));
        }
        
        for (int i = 0; i < 32; ++i)
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
        return 32;
    }
};
}


#endif /* w65c02_Stz_dpx_1_h */
