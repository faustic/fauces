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

import java.io.IOException;
import java.io.FileInputStream;

import com.faustic.cpu.wdc65x.Disassembler;
import com.faustic.memory.*;

public class Memory_image
{
    public static void main(String[] args)
	{
		System.out.println("Memory_image\n");
		Memory_image im = new Memory_image();
		
		int addr = 0;
		
		for (int i = 0; i < args.length; ++i)
		{
		   	try
	    	{
		   		addr = im.load_file(args[i], addr);
	    	}
	        catch(Exception e)
	        {
	            System.err.println("Failure: " + args[i] + ": " + 
	                                                            e.getMessage());
	            System.exit(1);
	        }
		}
		
		im.show();
	}
    
    private void show()
    {
    	Disassembler disas = new Disassembler(mem, 0x8100, 50, false);
    	disas.print();
    }
    
    private int load_file(String path, int addr) throws IOException
    {
    	byte[] bytes;
    	try(FileInputStream stream = new FileInputStream(path))
    	{
    		bytes = stream.readAllBytes();
    	}
		System.out.println(bytes.length + " bytes read\n");
		for (int i = 0; i < bytes.length; ++i)
    	{
    		mem.write8(addr++, bytes[i]);
    	}
    	return addr & 0xffff;
    }
    
   
    private Mem16_le_plain mem = new Mem16_le_plain();
    
}