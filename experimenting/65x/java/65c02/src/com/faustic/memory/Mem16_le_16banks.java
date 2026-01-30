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

public class Mem16_le_16banks extends Mem_le
{
    Mem16_le_16banks(Mem[] banks)
    {
        this.banks = banks;
        if (banks.length != 16)
            throw new IllegalArgumentException("");
    }
    
    @Override
    public int read8(int addr)
    {
        int index = bank_for_address(addr);
        return banks[index].read8(rel_address(index, addr)) & 0xff;
    }

    @Override
    public void write8(int addr, int value)
    {
        int index = bank_for_address(addr);
        banks[index].write8(rel_address(index, addr), value);        
    }
    
    int bank_for_address(int addr)
    {
        return (addr & 0xffff) >> 12;
    }
    
    int rel_address(int bank, int addr)
    {
        return (addr & 0xffff) - bank * 0x1000;
    }
    
    private Mem[] banks;

}
