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


#ifndef w65c02_Cmp_dpxi_1_h
#define w65c02_Cmp_dpxi_1_h
#include "../testgen.hpp"

namespace w65c02
{
class Cmp_dpxi_1: public Test
{
public:
    Cmp_dpxi_1(Mem& mem): Test(mem)
    {
        as.php();
        as.lda(Imm(0));
        as.pha();
        as.plp();

        for (int i = 0; i < 0x40; ++i)
        {
            as.lda(Abs(0x80 + i));
            as.sta(Abs(0x7000 + i));
        }
        for (int i = 224; i < 256; ++i)
        {
            as.lda(Imm(i));
            as.sta(Abs(0x7040 - 224 + i));
            as.lda(Imm(0x40 - 224 + i));
            as.sta(Abs(0x80 + (i - 224) * 2));
            as.lda(Imm(0x70));
            as.sta(Abs(0x80 + (i - 224) * 2 + 1));
        }
        as.ldx(Imm(0));
        for (int i = 0; i < 32; ++i)
        {
            string part = "part_";
            as.label(part + std::to_string(i));
            as.phx();
            as.pla();            
            as.phx();
            as.ldx(Imm(i * 2));
            as.cmp(Dpxi(0x80));
            as.plx();            
            as.sta(Absx(0x3000 + i * 256));
            as.php();
            as.pla();
            as.sta(Absx(0x5000 + i * 256));
            as.inx();
            as.bne(part + std::to_string(i));
        }
        for (int i = 0; i < 0x40; ++i)
        {
            as.lda(Abs(0x7000 + i));
            as.sta(Abs(0x80 + i));
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
        return 16384;
    }
};
}


#endif /* w65c02_Cmp_dpxi_1_h */
