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


#ifndef w65c02_Txs_1_h
#define w65c02_Txs_1_h
#include "../testgen.hpp"

namespace w65c02
{
class Txs_1: public Test
{
public:
    Txs_1(Mem& mem): Test(mem)
    {
        
        as.php();
        as.lda(Imm(0));
        as.pha();
        as.plp();
        as.lda(Dp(0xe0));
        as.sta(Abs(0x7000));
                
        int check[] =
        {
            0xB9, 0x69, 0xB9, 0x19, 0x29, 0xC5, 0x4D, 0xBB,
            0xAA, 0xFE, 0xFF, 0x86, 0xCE, 0x40, 0xE4, 0x6C
        };
        
        // Make sure the stack pointer is greater than 0x87
        as.tsx();
        as.stx(Dp(0xe0));
        as.lda(Imm(0x87));
        as.cmp(Dp(0xe0));
        as.bcc("test");
        as.brk(); // Break if stack pointer is too small

        // Test
        as.label("test");        
        for (int i = 0; i < 16; ++i)
        {
            as.ldx(Imm(0x87 - i));
            as.txs();
            as.php();
            as.pla();
            as.sta(Abs(0x3010 + i));
            as.lda(Imm(check[i]));
            as.pha();
            as.ldy(Abs(0x187 - i));
            as.sty(Abs(0x3000 + i));
        }
        
        
        // Move stack pointer to original position
        as.ldx(Dp(0xe0));
        as.txs();
        
        
        as.lda(Abs(0x7000));
        as.sta(Dp(0xe0));
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


#endif /* w65c02_Txs_1_h */
