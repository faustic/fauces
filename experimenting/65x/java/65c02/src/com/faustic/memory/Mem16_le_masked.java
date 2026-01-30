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

package com.faustic.memory;

public class Mem16_le_masked extends Mem_le
{
    @Override
    public int read8(int addr)
    {
        return data[addr & mask] & 0xff;
    }
    
    @Override
    public void write8(int addr, int value)
    {
        data[addr & mask] = (byte)value;
    }
    
    public Mem16_le_masked(int bits)
    {
        if (bits < 0)
            bits = 0;
        else if (bits > 16)
            bits = 16;
        int size = 1 << bits;
        data = new byte[size];
        mask = (~(0xffffffff << bits)) & 0xffff;
        System.out.println("mask: " + String.format("$%x", mask));
    }
    
    private byte[] data;
    private int mask;
}
