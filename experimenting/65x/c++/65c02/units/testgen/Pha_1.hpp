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


#ifndef w65c02_Pha_1_h
#define w65c02_Pha_1_h
#include "../testgen.hpp"

namespace w65c02
{
class Pha_1: public Test
{
public:
    Pha_1(Mem& mem): Test(mem)
    {
        as.php();
        as.lda(Imm(0));
        as.pha();
        as.plp();
        
        as.ldx(Imm(0));
        as.label("zeroloop");
        as.stz(Absx(0x3000));
        as.inx();
        as.bne("zeroloop");

        as.label("phaloop");
        as.phx();
        as.pla();
        as.pha();
        as.php();
        as.inx();
        as.inx();
        as.inx();
        as.inx();
        as.bne("phaloop");
        
        as.label("plaloop");
        as.pla();
        as.sta(Absx(0x3000));
        as.inx();
        as.pla();
        as.php();
        as.sta(Absx(0x3000));
        as.pla();
        as.inx();
        as.sta(Absx(0x3000));
        as.inx();
        as.inx();
        as.bne("plaloop");        
        
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
        return 256;
    }
};
}


#endif /* w65c02_Pha_1_h */
