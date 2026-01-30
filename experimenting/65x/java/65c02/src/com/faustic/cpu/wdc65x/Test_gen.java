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
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;

import com.faustic.cpu.wdc65x.test.Test;
import com.faustic.memory.*;

public class Test_gen
{
    public static void main(String[] args) throws IOException,
        ClassNotFoundException, InstantiationException, IllegalAccessException,
                            IllegalArgumentException, InvocationTargetException,
                                        NoSuchMethodException, SecurityException
    {
        System.out.println("TEST GENERATION");
        
        System.out.println(args.length);
        Mem16_le_plain mem = new Mem16_le_plain();
        for (int i = 0; i < args.length; ++i)
        {
            System.out.println(args[i]);
        
            if (args.length >= 1)
            {
                String class_name = args[i];
                String class_fullname = "com.faustic.cpu.wdc65x.test." +
                        class_name;
                Test test = (Test)Class.forName(class_fullname).
                        getDeclaredConstructor(Mem.class).newInstance(mem);
                System.out.println("Code start: " + test.code_start());
                System.out.println("Code size: " + test.code_size());
                System.out.println("Result start: " + test.result_start());
                System.out.println("Result size: " + test.result_size());
                System.out.println();
                
                if (test.code_size() > 4076)
                    throw new IllegalArgumentException();
                
                Executable exec = new Executable(mem, test.code_start(),
                    test.code_size(), test.result_start(), test.result_size());
                
                
                /*
                Disassembler disas = new Disassembler(mem, 0x2020, 600, true);
                disas.print();
                */
                
                String dir_name = "test/" + class_name;
                File dir = new File(dir_name);
                dir.mkdirs();
                String path = dir_name + "/TEST";
                exec.save(path);
            }
        }
    }
}
