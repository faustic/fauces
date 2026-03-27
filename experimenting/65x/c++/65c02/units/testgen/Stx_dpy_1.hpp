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


#ifndef w65c02_Stx_dpy_1_h
#define w65c02_Stx_dpy_1_h
#include "../testgen.hpp"

namespace w65c02
{
class Stx_dpy_1: public Test
{
public:
    Stx_dpy_1(Mem& mem): Test(mem)
    {
        as.php();
        
        int check[] =
        {
            0x72, 0xE5, 0x18, 0x63, 0xF8, 0x17, 0x42, 0x0B,
            0x9C, 0x92, 0xB5, 0x01, 0x63, 0xC7, 0xF4, 0x5D
        };
        
        for (int i = 0; i < 16; ++i)
        {
            as.lda(Dp(i));
            as.sta(Abs(0x7000 + i));
        }
        
        for (int i = 0; i < 16; ++i)
        {
            as.ldx(Imm(check[i]));
            as.ldy(Imm(i));
            as.stx(Dpy(0));
            as.ldx(Dpy(0));
            as.stx(Abs(0x3000 + i));
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


#endif /* w65c02_Stx_dpy_1_h */
