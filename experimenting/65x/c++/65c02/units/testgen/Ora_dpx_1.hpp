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


#ifndef w65c02_Ora_dpx_1_h
#define w65c02_Ora_dpx_1_h
#include "../testgen.hpp"

namespace w65c02
{
class Ora_dpx_1: public Test
{
public:
    Ora_dpx_1(Mem& mem): Test(mem)
    {
        as.php();
        as.lda(Imm(0));
        as.pha();
        as.plp();

        for (int i = 192; i < 224; ++i)
        {
            as.lda(Abs(0x80 - 192 + i));
            as.sta(Abs(0x7000 - 192 + i));
            as.lda(Imm(i));
            as.sta(Abs(0x80 - 192 + i));
        }
        as.ldx(Imm(0));
        for (int i = 0; i < 32; ++i)
        {
            string part = "part_";
            as.label(part + std::to_string(i));
            as.phx();
            as.pla();
            as.phx();
            as.ldx(Imm(i));
            as.ora(Dpx(0x80));
            as.plx();
            as.sta(Absx(0x3000 + i * 256));
            as.php();
            as.pla();
            as.sta(Absx(0x5000 + i * 256));
            as.inx();
            as.bne(part + std::to_string(i));
        }
        for (int i = 64; i < 96; ++i)
        {
            as.lda(Abs(0x7000 - 64 + i));
            as.sta(Abs(0x80 - 64 + i));
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


#endif /* w65c02_Ora_dpx_1_h */
