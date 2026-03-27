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


#ifndef w65c02_Stz_dp_1_h
#define w65c02_Stz_dp_1_h
#include "../testgen.hpp"

namespace w65c02
{
class Stz_dp_1: public Test
{
public:
    Stz_dp_1(Mem& mem): Test(mem)
    {
        as.php();
        
        int check[] =
        {
            0x9F, 0xD0, 0xB4, 0x4E, 0x70, 0xED, 0x41, 0x57,
            0xA0, 0xF5, 0xCE, 0xB4, 0x32, 0xCA, 0x61, 0xBC,
            0xE1, 0xDE, 0xE0, 0x2E, 0x74, 0x60, 0x41, 0xFC,
            0x94, 0x26, 0x98, 0x1F, 0xB3, 0xE8, 0x19, 0x72
        };
        
        for (int i = 0; i < 32; ++i)
        {
            as.lda(Dp(i));
            as.sta(Abs(0x7000 + i));
        }
        
        for (int i = 0; i < 32; ++i)
        {
            as.lda(Imm(check[i]));
            as.sta(Dp(i));
            if (i % 2 != 0)
                as.stz(Dp(i));
            as.lda(Dp(i));
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


#endif /* w65c02_Stz_dp_1_h */
