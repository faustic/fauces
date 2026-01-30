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

package com.faustic.cpu.wdc65x;


import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Arrays;

import com.faustic.memory.Mem;

public class Executable
{
    public class FormatException extends Exception
    {
        private static final long serialVersionUID = 8427407949632917975L;
        
    }
    
    public Executable(Mem mem, int start, int size, int result_start,
                                                                int result_size)
    {
        this.mem = mem;
        this.start = start;
        this.size = size;
        this.result_start = result_start;
        this.result_size = result_size;
    }
    
    public Executable(Mem mem, int start, int size)
    {
        this(mem, start, size, 0, 0);
    }
    
    public Executable(Mem mem)
    {
        this(mem, 0, 0);
    }
    
    public void save(String path) throws IOException
    {
        File file = new File(path);
        FileOutputStream stream = new FileOutputStream(file);
        stream.write(signature);
        stream.write((byte)(start));
        stream.write((byte)(start >> 8));
        stream.write((byte)(size));
        stream.write((byte)(size >> 8));
        stream.write((byte)(result_start));
        stream.write((byte)(result_start >> 8));
        stream.write((byte)(result_size));
        stream.write((byte)(result_size >> 8));
        for (int i = 0; i < 8; ++i)
            stream.write((byte)0);
        for (int i = 0; i < size; ++i)
            stream.write(mem.read8(start + i));
        stream.close();
    }
    
    private byte[] read_bytes(FileInputStream stream, int l)
                                            throws IOException, FormatException
    {
        byte[] bytes = stream.readNBytes(l);
        if(bytes.length != l)
        {
            stream.close();
            throw new FormatException();
        }
        return bytes;
    }
    
    private void bad_format(FileInputStream stream)
                                            throws IOException, FormatException
    {
        stream.close();
        throw new FormatException();
    }
    
    private int read_short(FileInputStream stream)
            throws IOException, FormatException
    {
        byte[] bytes = read_bytes(stream, 2);
        int n = (bytes[0] & 0xff) + 256 * (bytes[1] & 0xff);
        return n;
    }
    
    public void load(String path) throws IOException, FormatException
    {
        File file = new File(path);
        FileInputStream stream = new FileInputStream(file);
        byte[] signature = read_bytes(stream, 16);
        if (!Arrays.equals(signature, Executable.signature))
            bad_format(stream);
        start = read_short(stream);
        if (start == 0)
            start = 4128; 
        size = read_short(stream);
        if (start + size > 0xe000)
            bad_format(stream);
        result_start = read_short(stream);
        result_size = read_short(stream);
        read_short(stream);
        read_short(stream);
        read_short(stream);
        read_short(stream);
        
        byte[] bytes = stream.readNBytes(size);
        if (bytes.length != size)
            bad_format(stream);
        for (int i = start; i < start + size; ++i)
            mem.write8(i, bytes[i - start]);
        stream.close();
    }
    
    public int start()
    {
        return start;
    }
    
    public int result_start()
    {
        return result_start;
    }
    
    public int result_size()
    {
        return result_size;
    }
    
    private Mem mem;
    private int start;
    private int size;
    private int result_start;
    private int result_size;
    
    public static final byte[] signature =
    {
            (byte)0x3A, (byte)0x8D, (byte)0x90, (byte)0xF6,
            (byte)0xF9, (byte)0x92, (byte)0x40, (byte)0x45,
            (byte)0x83, (byte)0xB7, (byte)0xE6, (byte)0xAE,
            (byte)0x62, (byte)0x03, (byte)0x38, (byte)0x34
    };
}

