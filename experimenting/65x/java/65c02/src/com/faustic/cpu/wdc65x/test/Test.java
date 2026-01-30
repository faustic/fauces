/*
Licensed under the MIT License

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

package com.faustic.cpu.wdc65x.test;

import com.faustic.cpu.wdc65x.Assembler;
import com.faustic.memory.Mem;

public abstract class Test
{
    public Test(Mem mem)
    {
        this.mem = mem;
        as = new Assembler(mem, start);
    }
    
    public void end()
    {
        size = as.program_counter() - start;
        as.close();
    }
    
    public int code_size()
    {
        return size;
    }

    public int code_start()
    {
        return start;
    }

    public abstract int result_start();
    public abstract int result_size();
    
    static final int start = 0x2020;
    
    Mem mem;
    Assembler as;
    int size = 0;
    
}
