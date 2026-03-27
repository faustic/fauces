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


#ifndef w65c02_Sta_abs_1_h
#define w65c02_Sta_abs_1_h
#include "../testgen.hpp"

namespace w65c02
{
class Sta_abs_1: public Test
{
public:
    Sta_abs_1(Mem& mem): Test(mem)
    {
        as.php();
        
        int check[] =
        {
            0x68, 0x84, 0x35, 0xD3, 0x42, 0xAC, 0x45, 0x3F,
            0x8A, 0xFA, 0xFA,0xBF, 0xFF, 0x33, 0xED, 0xDA
        };
        
        for (int i = 0; i < 16; ++i)
        {
            as.lda(Imm(check[i]));
            as.sta(Abs(0x3000 + i));
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


#endif /* w65c02_Sta_abs_1_h */
