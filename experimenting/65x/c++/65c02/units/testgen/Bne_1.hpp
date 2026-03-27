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


#ifndef w65c02_Bne_1_h
#define w65c02_Bne_1_h
#include "../testgen.hpp"

namespace w65c02
{
class Bne_1: public Test
{
public:
    Bne_1(Mem& mem): Test(mem)
    {
        as.php();
        
        as.ldx(Imm(0));
        string loop = "loop";
        string branch = "branch";
        string save = "save";
        as.label(loop);
        as.phx();
        as.plp();
        as.bne(branch);
        as.lda(Imm(0x00));
        as.bra(save);
        as.label(branch);
        as.lda(Imm(0xff));
        as.label(save);
        as.sta(Absx(0x3000));
        as.php();
        as.pla();
        as.sta(Absx(0x3100));
        as.inx();
        as.bne(loop);

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
        return 512;
    }
};
}


#endif /* w65c02_Bne_1_h */
