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


#ifndef w65c02_Tax_1_h
#define w65c02_Tax_1_h
#include "../testgen.hpp"

namespace w65c02
{
class Tax_1: public Test
{
public:
    Tax_1(Mem& mem): Test(mem)
    {
        as.php();
        as.lda(Imm(0));
        as.pha();
        as.plp();
        
        int check[] =
        {
            0x2C, 0x7C, 0x0C, 0x5F, 0xDF, 0x0E, 0x4A, 0xD2,
            0x88, 0x12, 0x22, 0x6B, 0x12, 0x66, 0xA2, 0x16
        };
        
        for (int i = 0; i < 16; ++i)
        {
            as.lda(Imm(check[i]));
            as.ldx(Imm(0));
            as.tax();
            as.php();
            as.pla();
            as.stx(Abs(0x3000 + i));
            as.sta(Abs(0x3000 + 16 + i));
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


#endif /* w65c02_Tax_1_h */
