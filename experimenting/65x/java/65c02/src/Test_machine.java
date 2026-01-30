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

import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.WindowConstants;

import com.faustic.cpu.wdc65x.*;
import com.faustic.cpu.wdc65x.Assembler.Abs;
import com.faustic.cpu.wdc65x.Assembler.Imm;
import com.faustic.memory.*;

public class Test_machine
{
    class BadResultException extends Exception
    {
        private static final long serialVersionUID = -945693046014644515L;        
    }
    

    Test_machine(String test_path)
    {
        this(test_path, false, false, false, true);
    }

    Test_machine(String test_path, boolean ui, boolean rockwell, boolean pause,
            boolean fail)
    {
        this.test_path = test_path;
        
        if (fail)
            cpu.fail_on_unimplemented();
        else
            cpu.nop_on_unimplemented();
        cpu.rockwell_additions(rockwell);
        cpu.pause_additions(pause);
        
        if (ui)
            create_ui();
    }
    
    void create_ui()
    {
        String test =
                   this.test_path.substring(this.test_path.lastIndexOf('/') +1);
        frame = new JFrame(test);
        frame.setLayout(new FlowLayout());
        frame.setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
        JButton button = new JButton("Power off");
        ActionListener listener = new ActionListener()
                {
                    public void actionPerformed(ActionEvent ae)
                    {
                        power(false);
                    }
                };       
        WindowListener wlistener = new WindowAdapter()
                {
                    public void windowClosed(WindowEvent e)
                    {
                        power(false);
                    }
                };
        button.addActionListener(listener);
        frame.add(button);
        frame.addWindowListener(wlistener);
        button = new JButton("Reset");
        listener = new ActionListener()
        {
            public void actionPerformed(ActionEvent ae)
            {
                cpu.reset_low();
                cpu.reset_high();
            }
        };
        button.addActionListener(listener);
        frame.add(button);
        button = new JButton("IRQ");
        listener = new ActionListener()
        {
            public void actionPerformed(ActionEvent ae)
            {
                cpu.irq_low();
            }
        };
        button.addActionListener(listener);
        frame.add(button);
        button = new JButton("NMI");
        listener = new ActionListener()
        {
            public void actionPerformed(ActionEvent ae)
            {
                cpu.nmi_low();
                cpu.nmi_high();
            }
        };
        button.addActionListener(listener);
        frame.add(button);
        frame.pack();
        frame.setVisible(true);
    }

    void on() throws IOException, Executable.FormatException,
                                                   UnsupportedOperationException
    {

        Executable exec = new Executable(mem);
        exec.load(this.test_path + "/TEST");
        
        this.result_address = exec.result_start();
        this.result_size = exec.result_size();
        
        int start = 0xf000;
        Assembler as = new Assembler(mem, start);

        as.stz(Abs.addr(report_res));
        as.bra("poweroff");

        mem.write16(0xfffc, as.program_counter()); // Initial reset vector
        as.lda(Imm.val(start)); // Runtime reset vector
        as.sta(Abs.addr(0xfffc));
        as.lda(Imm.val(start >> 8));
        as.sta(Abs.addr(0xfffd));
        as.cli();
        as.jsr(Abs.addr(exec.start())); // Jump to test code
        
        as.label("poweroff");
        as.stz(Abs.addr(poweroff));
        as.label("wait_poweroff");
        as.bra("wait_poweroff");
        
        mem.write16(0xfffe, as.program_counter()); // Break/IRQ vector
        as.pla();
        as.pha();
        as.and(Imm.val(1 << 4));
        as.bne("brk_handler");
        as.stz(Abs.addr(report_irq));
        as.stz(Abs.addr(interrupt_done));
        as.rti();
        as.label("brk_handler");
        as.stz(Abs.addr(report_brk));
        as.bra("poweroff");
        
        mem.write16(0xfffa, as.program_counter()); // NMI vector
        as.stz(Abs.addr(report_nmi));
        as.bra("poweroff");
        
/*
        Disassembler disas = new Disassembler(mem, 0x2020, 600, true);
        disas.print();
*/
        power(true);
        cpu.reset_low();
        cpu.on();
        cpu.reset_high();
        while (on)
        {
            try
            {
                Thread.sleep(1);
                if (cpu.failed())
                    throw new UnsupportedOperationException(cpu.fail_msg());
            }
            catch(InterruptedException e)
            {
                power(false);
            }
        }
        try
        {
            off();
        }
        catch(BadResultException e)
        {
            Test_machine.fail();
            System.out.println("Test failed\n");
        }
    }
    
    void power(boolean on)
    {
        this.on = on;
    }
    
    void irq_clear()
    {
        cpu.irq_high();
    }

    void off() throws IOException, BadResultException
    {
        if (frame != null)
        {
            frame.dispose();
            frame = null;
        }
        if (!cpu.failed())
        {
            cpu.off();
            compare();
        }
        int data_view= 0xffff;
        if (data_view != 0xffff)
        {
            for (int i = 0; i < 16; ++i)
            {
                System.out.print(String.format("%04X: ", data_view + i * 16));
                for (int j = 0; j < 16; ++j)
                    System.out.print(String.format("%02X ",
                                            mem.read8(data_view + i * 16 + j)));
                System.out.print("  ");
                for (int j = 0; j < 16; ++j)
                {
                    int v = mem.read8(data_view + i * 16 + j);
                    if (v >= 0x20 && v <= 0x7f)
                        System.out.print((char)v);
                    else
                        System.out.print(".");
                }
                System.out.println();
            }
        }
    }
    
    private void compare() throws IOException, BadResultException
    {
        if (result_size > 0)
        {
            File file = new File(this.test_path + "/RESULT");
            FileInputStream stream = new FileInputStream(file);
            byte[] bytes = stream.readNBytes(result_size);
            stream.close();
            if(bytes.length != result_size)
                throw new BadResultException();
            for (int i = 0; i < result_size; ++i)
            {
                if (bytes[i] != (byte)mem.read8(result_address + i))
                    throw new BadResultException();
                    
            }
        }
        System.out.println("Test OK\n");
    }

    public static void main(String[] args) throws IOException,
                                                    Executable.FormatException
    {
        System.out.println("Test_machine\n");
        boolean ui = false;
        boolean pausable = false;
        boolean rockwell = false;
        boolean fail = true;
        for (int i = 0; i < args.length && ! fatal_error; ++i)
        {
            switch (args[i])
            {
                case "--ui":
                    ui = true;
                    continue;
                case "--no-ui":
                    ui = false;
                    continue;
                case "--rockwell":
                    rockwell = true;
                    continue;
                case "--no-rockwell":
                    rockwell = false;
                    continue;
                case "--pausable":
                    pausable = true;
                    continue;
                case "--no-pausable":
                    pausable = false;
                    continue;
                case "--fail":
                    fail = true;
                    continue;
                case "--no-fail":
                    fail = false;
                    continue;
            }
            try
            {
                Test_machine machine = new Test_machine(args[i], ui, rockwell,
                        pausable, fail);
                System.out.println(args[i]);
                machine.on();
            }
            catch(Exception e)
            {
                System.err.println("Failure: " + args[i] + ": " + 
                                                                e.getMessage());
                System.exit(1);
            }
        }
        System.exit(0); // Makes sure the program does not remain open because
                        // of the user interface
    }
    
    public static void fail()
    {
        fatal_error = true;
    }
    
    public Cpu65c02 cpu()
    {
        return cpu;
    }

    private Mem mem = new Mem_test(this);
    private Cpu65c02 cpu = new Cpu65c02(mem);
    private String test_path;
    private int result_size = 0;
    private int result_address = 0;
    private static boolean fatal_error = false;
    private JFrame frame = null;
    private volatile boolean on = false;
    
    // Host-guest interface
    public static final int poweroff = 0xe000;
    public static final int report_brk = 0xe001;
    public static final int report_res = 0xe002;
    public static final int report_irq = 0xe003;
    public static final int report_nmi = 0xe004;
    public static final int interrupt_done = 0xe005;
}

class Mem_test extends Mem16_le_plain
{
    Mem_test(Test_machine machine)
    {
        this.machine = machine;
    }

    @Override
    public void write8(int addr, int value)
    {
        addr &= 0xffff;
        super.write8(addr, value);
        switch (addr)
        {
            case Test_machine.poweroff:
                machine.power(false);
                break;
            case Test_machine.report_brk:
            {
                int s = machine.cpu().s();
                int p = read8(0x100 + ((s + 1) & 0xff));
                int pc = read16(0x100 + ((s + 2) & 0xff));
                System.out.println(String.format("BRK", p));
                System.out.println(String.format("P: $%02x", p));
                System.out.println(String.format("PC: $%04x", pc));
                break;
            }
            case Test_machine.report_res:
                System.out.println("RES");
                break;
            case Test_machine.report_irq:
                System.out.println("IRQ");
                break;
            case Test_machine.report_nmi:
                System.out.println("NMI");
                break;
            case Test_machine.interrupt_done:
                System.out.println("IRQ handled");
                machine.irq_clear();
                break;
        }
    }

    private Test_machine machine;

}
