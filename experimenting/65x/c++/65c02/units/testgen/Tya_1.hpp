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


#ifndef w65c02_Tya_1_h
#define w65c02_Tya_1_h
#include "../testgen.hpp"

namespace w65c02
{
class Tya_1: public Test
{
public:
    Tya_1(Mem& mem): Test(mem)
    {
        as.php();
        as.lda(Imm(0));
        as.pha();
        as.plp();
        
        int check[] =
        {
            0x56, 0x50, 0x5D, 0x5C, 0x7E, 0x8E, 0x42, 0x7A,
            0xBA, 0x2C, 0xA0, 0x99, 0x6E, 0xAD, 0xDF, 0x37
        };
        
        for (int i = 0; i < 16; ++i)
        {
            as.ldy(Imm(check[i]));
            as.lda(Imm(0));
            as.tya();
            as.php();
            as.ply();
            as.sta(Abs(0x3000 + i));
            as.sty(Abs(0x3000 + 16 + i));
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


#endif /* w65c02_Tya_1_h */
