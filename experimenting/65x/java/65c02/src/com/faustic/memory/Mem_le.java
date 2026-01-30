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

public abstract class Mem_le implements Mem
{
    @Override
    public int read16(int addr)
    {
        return ((read8((addr + 1)) << 8) | read8(addr)) & 0xffff;
    }
    
    @Override
    public int read32(int addr)
    {
        return (read16((addr + 2)) << 16) | read16(addr);
    }
    
    @Override
    public long read64(int addr)
    {
        return ((long)read32((addr + 4)) << 32) | read32(addr);
    }
    
    @Override
    public void write16(int addr, int value)
    {
        write8(addr, (byte)value);
        write8((addr + 1), (byte)(value >> 8));
    }
    
    @Override
    public void write32(int addr, int value)
    {
        write16(addr, (short)value);
        write16((addr + 2), (short)(value >> 16));
    }

    @Override
    public void write64(int addr, long value)
    {
        write32(addr, (int)value);
        write32((addr + 4), (int)(value >> 32));
    }
}
