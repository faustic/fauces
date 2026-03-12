// Generate test programs for the 65c02 emulator
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


#ifndef w65c02_testgen_hpp
#define w65c02_testgen_hpp

#include "assembler.hpp"

namespace w65c02
{

class Test
{
public:
    Test(Mem& mem):
    mem(mem), as(mem, start)
    {
    }
    
    virtual ~Test() = default;
    
    virtual void end()
    {
        size = as.program_counter() - start;
        as.close();
    }
    
    virtual size_t code_size()
    {
        return size;
    }
    
    virtual Address code_start()
    {
        return start;
    }
    
    virtual Address result_start() = 0;
    virtual size_t result_size() = 0;
    
protected:
    Assembler as;

private:
    static constexpr Address start = 0x2020;
    Mem& mem;
    size_t size = 0;
};

}


#endif /* w65c02_testgen_hpp */
