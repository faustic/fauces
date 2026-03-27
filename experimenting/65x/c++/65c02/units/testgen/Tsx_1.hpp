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


#ifndef w65c02_Tsx_1_h
#define w65c02_Tsx_1_h
#include "../testgen.hpp"

namespace w65c02
{
class Tsx_1: public Test
{
public:
    Tsx_1(Mem& mem): Test(mem)
    {
        
        as.php();
        as.lda(Imm(0));
        as.pha();
        as.plp();
        as.lda(Dp(0xe0));
        as.sta(Abs(0x7000));
                
        int check[] =
        {
            0x83, 0x44, 0x68, 0xEB, 0xC2, 0x60, 0x4C, 0x0F,
            0x8E, 0x87, 0x3D, 0xBB, 0x8C, 0x63, 0x5E, 0xEB
        };
        
        // Make sure the stack pointer is greater than 0x87
        as.tsx();
        as.stx(Dp(0xe0));
        as.lda(Imm(0x87));
        as.cmp(Dp(0xe0));
        as.bcc("position_stack");
        as.brk(); // Break if stack pointer is too small

        // Move stack pointer to 0x87
        as.ldy(Imm(0));
        as.label("position_stack");
        as.phy(); // Just to decrement the stack (pushed value will be unused)
        as.tsx();
        as.cpx(Imm(0x87));
        as.beq("stack_ok");
        as.bra("position_stack");
        
        // Save check data
        as.label("stack_ok");
        // Now S == 0x87
        for (int i = 0; i < 16; ++i)
        {
            as.lda(Imm(check[i]));
            as.pha();
        }
        // Now S == 0x77
        
        // Test
        as.label("test");
        as.tsx();
        as.php();
        as.pla();
        as.sta(Absx(0x3010 - 0x77)); // Save status from TSX operation
        as.lda(Absx(0x100 + 1));
        as.sta(Absx(0x3000 - 0x77));
        as.ply(); // Just to increment the stack (Y will be unused)
        as.cpx(Imm(0x87 - 1));
        as.bne("test");
        
        // Move stack pointer to original position
        as.label("restore_stack");
        as.pla();
        as.tsx();
        as.cpx(Dp(0xe0));
        as.bne("restore_stack");
        
        
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


#endif /* w65c02_Tsx_1_h */
