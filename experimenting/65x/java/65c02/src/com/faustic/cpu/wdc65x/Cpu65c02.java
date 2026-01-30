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

import com.faustic.memory.Mem;

public class Cpu65c02 implements Processor
{
    public Cpu65c02(Mem mem, int period)
    {
        this.mem = mem;
        this.period = period;
        parser = new Parser(this, mem);
    }
    
    public Cpu65c02(Mem mem)
    {
        this(mem, 0);
    }
    
    public synchronized void on()
    {
        if (!vdd && power == null)
        {
            failed = false;
            fail_msg = "";
            vdd = true;
            power = new Power();
        }
    }
    
    public synchronized void off()
    {
        if (vdd && power != null)
        {
            vdd = false;
            power.wait_off();
            power = null;
        }
    }
    
    public void reset_high()
    {
        boolean was_low = !resb_high;
        resb_high = true;
        if (was_low)
        {
            resb_sequence = true;
            update_attention();
        }
    }
    
    public void reset_low()
    {
        resb_high = false;        
    }
    
    public void nmi_high()
    {
        nmi_high = true;
    }
    
    public void nmi_low()
    {
        boolean was_high = nmi_high;
        nmi_high = false;        
        if (was_high)
        {
            nmi_sequence = true;
            update_attention();
        }
    }
    
    public void irq_low()
    {
        irqline_low = true;
        update_attention();
    }
    
    public void irq_high()
    {
        irqline_low = false;
        update_attention();
    }
    
    public void change_period(int nanosec)
    {
        period = nanosec;
    }
    
    public void fail_on_unimplemented()
    {
        fail_on_unimplemented = true;
    }
    
    public boolean failed()
    {
        return failed;
    }
    
    public String fail_msg()
    {
        return fail_msg;
    }
    
    public void nop_on_unimplemented()
    {
        fail_on_unimplemented = false;
    }
    
    public void rockwell_additions(boolean enable)
    {
        rockwell = enable;
    }
    
    public void pause_additions(boolean enable)
    {
        pausable = enable;
    }
    
    public byte s()
    {
        return s;
    }
    
    private class Power implements Runnable
    {
        Thread thread;
        public Power()
        {
            thread = new Thread(this);
            thread.start();
        }
        public void run()
        {
            on_sequence();
        }
        public void wait_off()
        {
            if (Thread.currentThread() != thread)
            {
                try
                {
                    thread.join();
                }
                catch(InterruptedException e)
                {
                    // Ignore. We are already finishing.
                }
            }
        }
    }
    
    private void on_sequence()
    {
// Unused bit (5) of P appears to be always high in a real 65C02.
// Also, it seems bit b(4) of P is never really clear in the register itself,
// but only in its pushed copy in the stack after a hardware interrupt.
        p |= u_mask | b_mask;
        while (!resb_sequence)
        {
            try
            {
                Thread.sleep(0, period != 0 ? period: 1000);
            }
            catch(InterruptedException e)
            {
                vdd = false;
                fail_msg = e.getMessage();
                failed = true;
                return;
            }
        }
        loop();
    }
    
    private void loop()
    {
        update_attention();
        try
        {
            while (vdd)
            {
                if (!resb_high)
                    continue;
                if (attention_needed)
                    attend();
                pc = parser.parse(pc);           
            }
        }
        catch(UnsupportedOperationException e)
        {
            vdd = false;
            fail_msg = e.getMessage();
            failed = true;
        }
        a = 0;
        x = 0;
        y = 0;
        p = 0;
        s = 0;
        pc = 0;
   }
    
    private void attend()
    {
        if (resb_sequence)
        {
            nmi_sequence = false;
            resb_sequence = false;
            handle_interrupt(0xfffc, p);
        }
        else if(nmi_sequence)
        {
            nmi_sequence = false;
            handle_interrupt(0xfffa, p);
        }
        else if (irqline_low && (p & i_mask) == 0)
            handle_interrupt(0xfffe, p & ~b_mask);
        update_attention();
    }
    
    private void update_attention()
    {
        attention_needed = resb_sequence || nmi_sequence ||
                                               irqline_low && (p & i_mask) == 0;
    }
    
    private void cycles(int n)
    {
        if (period > 0)
        {
            long expected_time = last_time + (long)n * period;
            long current_time = System.nanoTime();
            while (current_time < expected_time && current_time >= last_time)
            {
                long wait = expected_time - current_time;
                long ms = wait / 1000000;
                int ns = (int)(wait % 1000000);
                try
                {
                    Thread.sleep(ms, ns);
                }
                catch(InterruptedException e)
                {
                    vdd = false;
                    fail_msg = e.getMessage();
                    failed = true;
                    return;
                }
                current_time = System.nanoTime();
            }
            last_time = expected_time;
        }
    }
    
    private void handle_interrupt(int vector, int status)
    {
        {
            // Push program counter and status register when IRQ or NMI happen.
            
            // We also push them on RESET. As of 2020, the 65C02S data sheet
            // does not mention this behaviour, but "Programming the 65816"
            // claims that this happens with the 65C02, in contrast with the
            // 6502. 
            
            mem.write16(push16(), pc);
            mem.write8(push(), status);
        }
        p |= i_mask;
        p &= ~d_mask;
        pc = mem.read16(vector);
        cycles(7);
    }
    
    private int page_crossed(int addr, int index)
    {
        addr = mem.read16(addr);
        if ((addr & 0xff00) != ((addr + index) & 0xff00))
            return 1;
        return 0;
    }
    
    private int page_crossed_zindirect(int addr, int index)
    {
        addr = mem.read16(mem.read8(addr));
        if ((addr & 0xff00) != ((addr + index) & 0xff00))
            return 1;
        return 0;
    }
    
    private int decimal_set()
    {
        return ((p & d_mask) != 0) ? 1: 0;
    }
    
    private int pull()
    {
        return 0x100 + (++s & 0xff);
    }
    
    private int pull16()
    {
        int value_addr = 0x100 + ((s + 1) & 0xff);
        s += 2;
        return value_addr;
    }
    
    private int push()
    {
        return 0x100 + (s-- & 0xff);
    }
    
    private int push16()
    {
        int value_addr = 0x100 + ((s - 1) & 0xff); 
        s -= 2;
        return value_addr;
    }
    
    private int sum_dec(int v1, int v2)
    {
        int c = (p & c_mask) == 0 ? 0 : 1;
        p &= ~c_mask;
        int p1 = v1 & 0xf;
        int p2 = v2 & 0xf;
        int result = p1 + p2 + c;
        c = 0;
        if (result >= 0xa)
        {
            result = (result - 0xa) & 0xf;
            c = 0x10;
        }
        p1 = v1 & 0xf0;
        p2 = v2 & 0xf0;
        int rt = p1 + p2 + c;
        c = 0;
        if (rt >= 0xa0)
        {
            rt -= 0xa0;
            p |= c_mask;
        }
        result |= rt;
        p &= ~v_mask;
        byte b1 = (byte)v1;
        byte b2 = (byte)v2;
        byte br = (byte)result;
        if (b1 <= 0 && b2 <= 0)
        {
            if (br > 0 || br > b1 || br > b2)
                p |= v_mask;
        }
        else if (b1 >= 0 && b2 >= 0)
        {
            if (br < 0 || br < b1 || br < b2)
                p |= v_mask;
        }
        update_flags_n_z(result);
        return result;
    }
    
    private int sum_bin(int v1, int v2)
    {
        int result;
        int c = (p & c_mask) == 0 ? 0 : 1;
        result = (v1 & 0xff) + (v2 & 0xff) + c;
        p &= ~c_mask;
        if (result >= 0x100)
            p |= c_mask;
        p &= ~v_mask;
        result = (byte)v1 + (byte)v2 + c;
        byte br = (byte)result;
        if ((br < 0) != (result < 0))
            p |= v_mask;
        update_flags_n_z(result);
        return result;
    }
    
    private int sum(int v1, int v2)
    {
        if ((p & d_mask) != 0)
            return sum_dec(v1, v2);
        else
            return sum_bin(v1, v2);
    }
    
    private void asl_ea(int ea)
    {
        int value = mem.read8(ea);
        p &= ~c_mask;
        p |= (value >> (7 - c_bit)) & c_mask;
        value <<= 1;
        mem.write8(ea, value);
        update_flags_n_z(value);        
    }
    
    private void bit(int value)
    {
        p &= ~(n_mask | v_mask | z_mask);
        p |= value & (n_mask | v_mask);
        if ((value & a & 0xff) == 0)
            p |= z_mask;
    }
    
    private int bbr(int addr, int bit) // 0 < bit <= 7
    {
        if (rockwell)
        {
            int dst_addr = mode_dp_ea(addr++);
            int imm = (byte)mem.read8(addr++);
            int value = mem.read8(dst_addr);
            if ((value & (1 << bit)) == 0)
                addr += imm;
            cycles(5);
            return addr;    
        }
        else
            return unimplemented_1_1(addr);
    }

    private int bbs(int addr, int bit) // 0 < bit <= 7
    {
        if (rockwell)
        {
            int dst_addr = mode_dp_ea(addr++);
            int imm = (byte)mem.read8(addr++);
            int value = mem.read8(dst_addr);
            if ((value & (1 << bit)) != 0)
                addr += imm;
            cycles(5);
            return addr;    
        }
        else
            return unimplemented_1_1(addr);
    }

    private void cmp(int v1, int v2)
    {
        int result;
        v1 &= 0xff;
        v2 &= 0xff;
        result = v1 - v2;
        p &= ~c_mask;
        if (v2 <= v1)
            p |= c_mask;
        update_flags_n_z(result);
    }
    
    private void dec_ea(int ea)
    {
        int value = mem.read8(ea);
        mem.write8(ea, --value);
        update_flags_n_z(value);        
    }
    
    private void inc_ea(int ea)
    {
        int value = mem.read8(ea);
        mem.write8(ea, ++value);
        update_flags_n_z(value);        
    }
    
    private void lsr_ea(int ea)
    {
        int value = mem.read8(ea);
        p &= ~c_mask;
        p |= (value << c_bit) & c_mask;
        value >>= 1;
        mem.write8(ea, value);
        update_flags_n_z(value);        
    }
    
    private int rmb(int addr, int bit) // 0 < bit <= 7
    {
        if (rockwell)
        {
            int dst_addr = mode_dp_ea(addr);
            int value = mem.read8(dst_addr);
            mem.write8(dst_addr, value & ~(1 << bit));
            cycles(5);
            return addr + 1;    
        }
        else
            return unimplemented_1_1(addr);
    }

    private void rol_ea(int ea)
    {
        int value = mem.read8(ea);
        int rbit = (p & c_mask) >> c_bit;
        p &= ~c_mask;
        p |= (value & 0x80) >> (7 - c_bit);
        value <<= 1;
        value |= rbit;
        mem.write8(ea, value);
        update_flags_n_z(value);        
    }
    
    private void ror_ea(int ea)
    {
        int value = mem.read8(ea);
        int lbit = (p & c_mask) << (7 - c_bit);
        p &= ~c_mask;
        p |= (value & 1) << c_bit;
        value >>= 1;
        value |= lbit;
        mem.write8(ea, value);
        update_flags_n_z(value);        
    }
    
    private int smb(int addr, int bit) // 0 < bit <= 7
    {
        if (rockwell)
        {
            int dst_addr = mode_dp_ea(addr);
            int value = mem.read8(dst_addr);
            mem.write8(dst_addr, value | (1 << bit));
            cycles(5);
            return addr + 1;    
        }
        else
            return unimplemented_1_1(addr);
    }

    private void sub_overflow(int v1, int v2, int c)
    {
        p &= ~v_mask;
        int result = (byte)v1 - (byte)v2 - c;
        byte br = (byte)result;
        if ((br < 0) != (result < 0))
            p |= v_mask;
    }
    
    private int sub_dec(int v1, int v2)
    {
        int c = (p & c_mask) == 0 ? 1 : 0;
        sub_overflow(v1, v2, c);
        p |= c_mask;
        int p1 = v1 & 0xf;
        int p2 = v2 & 0xf;
        int result = p1 - p2 - c;
        c = 0;
        int c_bad = 0;
        if (result < 0)
        {
            result += 0xa;
            c += 0x10;
            if (result < 0)
                c_bad= 0x10;
            result &= 0xf;
        }
        p1 = v1 & 0xf0;
        p2 = v2 & 0xf0;
        int rt = p1 - p2 - c;
        if (rt < 0)
        {
            rt += 0xa0;
            p &= ~c_mask;
        }
        rt -= c_bad;
        result |= rt;        
        update_flags_n_z(result);
        return result;
    }
    
    private int sub_bin(int v1, int v2)
    {
        int result;
        int c = (p & c_mask) == 0 ? 1 : 0;
        result = (v1 & 0xff) - (v2 & 0xff) - c;
        p |= c_mask;
        if (result < 0)
            p &= ~c_mask;
        sub_overflow(v1, v2, c);
        update_flags_n_z(result);
        return result;
    }
    
    private int sub(int v1, int v2)
    {
        if ((p & d_mask) != 0)
            return sub_dec(v1, v2);
        else
            return sub_bin(v1, v2);
    }
    
    private void update_flags_n_z(int value)
    {
        p &= ~(n_mask | z_mask);
        if ((value & 0x80) != 0)
            p |= n_mask;
        if ((value & 0xff) == 0)
            p |= z_mask;
    }
    
    private int mode_abs_ea(int addr)
    {
        return mem.read16(addr);
    }
    
    private int mode_abs(int addr)
    {
        return mem.read8(mode_abs_ea(addr));
    }
    
    private int mode_abs_x_ea(int addr)
    {
        return mem.read16(addr) + (x & 0xff);
    }
    
    private int mode_abs_x(int addr)
    {
        return mem.read8(mode_abs_x_ea(addr));
    }
    
    private int mode_abs_y_ea(int addr)
    {
        return mem.read16(addr) + (y & 0xff);
    }
    
    private int mode_abs_y(int addr)
    {
        return mem.read8(mode_abs_y_ea(addr));
    }
    
    private int mode_dp_ea(int addr)
    {
        return mem.read8(addr);
    }
    
    private int mode_dp(int addr)
    {
        return mem.read8(mode_dp_ea(addr));
    }
    
    private int mode_dp___ea(int addr)
    {
        return mem.read16(mem.read8(addr));
    }
    
    private int mode_dp__(int addr)
    {
        return mem.read8(mode_dp___ea(addr));
    }
    
    private int mode_dp_x_ea(int addr)
    {
        return (mem.read8(addr) + x) & 0xff;
    }
    
    private int mode_dp_x(int addr)
    {
        return mem.read8(mode_dp_x_ea(addr));
    }
    
    private int mode_dp_y_ea(int addr)
    {
        return (mem.read8(addr) + y) & 0xff;
    }
    
    private int mode_dp_y(int addr)
    {
        return mem.read8(mode_dp_y_ea(addr));
    }
    
    private int mode_dp_x___ea(int addr)
    {
        return mem.read16((mem.read8(addr) + x) & 0xff);
    }
    
    private int mode_dp_x__(int addr)
    {
        return mem.read8(mode_dp_x___ea(addr));
    }
    
    private int mode_dp___y_ea(int addr)
    {
        return mem.read16(mem.read8(addr)) + (y & 0xff);
    }
    
    private int mode_dp___y(int addr)
    {
        return mem.read8(mode_dp___y_ea(addr));
    }
    
    private void unimplemented_error(int addr)
    {
        if (fail_on_unimplemented)
        {
            int inst_addr = addr - 1;
            throw new UnsupportedOperationException
            (
                String.format
                (
                    "Unimplemented instruction at $%04x: $%02x",
                    inst_addr,
                    mem.read8(inst_addr)
                )
            );
        }
    }
    
    public int unimplemented_2_2(int addr)
    {
        unimplemented_error(addr);
        cycles(2);
        return addr + 1;
    }
    
    public int unimplemented_1_1(int addr)
    {
        unimplemented_error(addr);
        cycles(1);
        return addr;
    }
    
    public int unimplemented_2_3(int addr)
    {
        unimplemented_error(addr);
        cycles(3);
        return addr + 1;
    }
    
    public int unimplemented_2_4(int addr)
    {
        unimplemented_error(addr);
        cycles(4);
        return addr + 1;
    }
    
    public int unimplemented_3_8(int addr)
    {
        unimplemented_error(addr);
        cycles(8);
        return addr + 2;
    }
    
    public int unimplemented_3_4(int addr)
    {
        unimplemented_error(addr);
        cycles(4);
        return addr + 2;
    }
    
    @Override
    public int adc_abs(int addr)
    {
        a = (byte)sum(a, mode_abs(addr));
        cycles(4);
        cycles(decimal_set());
        return addr + 2;    
    }

    @Override
    public int adc_abs_x(int addr)
    {
        a = (byte)sum(a, mode_abs_x(addr));
        cycles(4 + decimal_set() + page_crossed(addr, x & 0xff));
        return addr + 2;
    }

    @Override
    public int adc_abs_y(int addr)
    {
        a = (byte)sum(a, mode_abs_y(addr));
        cycles(4 + decimal_set() + page_crossed(addr, y & 0xff));
        return addr + 2;
    }

    @Override
    public int adc_abslong(int addr)
    {
        return bbr(addr, 6);
    }
    
    @Override
    public int adc_abslong_x(int addr)
    {
        return bbr(addr, 7);
    }
    
    @Override
    public int adc_dp(int addr)
    {
        a = (byte)sum(a, mode_dp(addr));
        cycles(3 + decimal_set());
        return addr + 1;    
    }

    @Override
    public int adc_dp__(int addr)
    {
        a = (byte)sum(a, mode_dp__(addr));
        cycles(5 + decimal_set());
        return addr + 1;
    }

    @Override
    public int adc_dp_x(int addr)
    {
        a = (byte)sum(a, mode_dp_x(addr));
        cycles(4 + decimal_set());
        return addr + 1;    
    }

    @Override
    public int adc_dp_x__(int addr)
    {
        a = (byte)sum(a, mode_dp_x__(addr));
        cycles(6 + decimal_set());
        return addr + 1;
    }

    @Override
    public int adc_dp___y(int addr)
    {
        a = (byte)sum(a, mode_dp___y(addr));
        cycles(5 + decimal_set() + page_crossed_zindirect(addr, y & 0xff));
        return addr + 1;
    }

    @Override
    public int adc_dp__long(int addr)
    {
        return rmb(addr, 6);
    }

    @Override
    public int adc_dp__long_y(int addr)
    {
        return rmb(addr, 7);
    }

    @Override
    public int adc_imm(int addr)
    {
        a = (byte)sum(a, mem.read8(addr));
        cycles(2 + decimal_set());
        return addr + 1;
    }

    @Override
    public int adc_sr(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int adc_sr__y(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int and_abs(int addr)
    {
        a &= mode_abs(addr);
        update_flags_n_z(a);
        cycles(4);
        return addr + 2;    
    }

    @Override
    public int and_abs_x(int addr)
    {
        a &= mode_abs_x(addr);
        update_flags_n_z(a);
        cycles(4 + page_crossed(addr, x & 0xff));
        return addr + 2;
    }

    @Override
    public int and_abs_y(int addr)
    {
        a &= mode_abs_y(addr);
        update_flags_n_z(a);
        cycles(4 + page_crossed(addr, y & 0xff));
        return addr + 2;
    }

    @Override
    public int and_abslong(int addr)
    {
        return bbr(addr, 2);
    }
    
    @Override
    public int and_abslong_x(int addr)
    {
        return bbr(addr, 3);
    }
    
    @Override
    public int and_dp(int addr)
    {
        a &= mode_dp(addr);
        update_flags_n_z(a);
        cycles(3);
        return addr + 1;    
    }

    @Override
    public int and_dp__(int addr)
    {
        a &= mode_dp__(addr);
        update_flags_n_z(a);
        cycles(5);
        return addr + 1;
    }

    @Override
    public int and_dp_x(int addr)
    {
        a &= mode_dp_x(addr);
        update_flags_n_z(a);
        cycles(4);
        return addr + 1;    
    }

    @Override
    public int and_dp_x__(int addr)
    {
        a &= mode_dp_x__(addr);
        update_flags_n_z(a);
        cycles(6);
        return addr + 1;
    }

    @Override
    public int and_dp___y(int addr)
    {
        a &= mode_dp___y(addr);
        update_flags_n_z(a);
        cycles(5 + page_crossed_zindirect(addr, y & 0xff));
        return addr + 1;
    }

    @Override
    public int and_dp__long(int addr)
    {
        return rmb(addr, 2);
    }

    @Override
    public int and_dp__long_y(int addr)
    {
        return rmb(addr, 3);
    }

    @Override
    public int and_imm(int addr)
    {
        a &= mem.read8(addr);
        update_flags_n_z(a);
        cycles(2);
        return addr + 1;
    }

    @Override
    public int and_sr(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int and_sr__y(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int asl_abs(int addr)
    {
        asl_ea(mode_abs_ea(addr));
        cycles(6);
        return addr + 2;    
    }

    @Override
    public int asl_abs_x(int addr)
    {
        asl_ea(mode_abs_x_ea(addr));
        cycles(6 + page_crossed(addr, x & 0xff));
        return addr + 2;    
    }

    @Override
    public int asl_acc(int addr)
    {
        p &= ~c_mask;
        p |= (a >> (7 - c_bit)) & c_mask;
        a <<= 1;
        update_flags_n_z(a);
        cycles(2);
        return addr;
    }

    @Override
    public int asl_dp(int addr)
    {
        asl_ea(mode_dp_ea(addr));
        cycles(5);
        return addr + 1;    
    }

    @Override
    public int asl_dp_x(int addr)
    {
        asl_ea(mode_dp_x_ea(addr));
        cycles(6);
        return addr + 1;    
    }

    @Override
    public int bcc(int addr)
    {
        addr += 1;
        if ((p & c_mask) == 0)
        {
            int imm = (byte)mem.read8(addr - 1);
            addr += imm;
            cycles(1 + page_crossed(addr, -imm));
        }
        cycles(2);
        return addr;
    }

    @Override
    public int bcs(int addr)
    {
        addr += 1;
        if ((p & c_mask) != 0)
        {
            int imm = (byte)mem.read8(addr - 1);
            addr += imm;
            cycles(1 + page_crossed(addr, -imm));
        }
        cycles(2);
        return addr;
    }

    @Override
    public int beq(int addr)
    {
        addr += 1;
        if ((p & z_mask) != 0)
        {
            int imm = (byte)mem.read8(addr - 1);
            addr += imm;
            cycles(1 + page_crossed(addr, -imm));
        }
        cycles(2);
        return addr;
    }

    @Override
    public int bit_abs(int addr)
    {
        bit(mode_abs(addr));
        cycles(4);
        return addr + 2;
    }

    @Override
    public int bit_abs_x(int addr)
    {
        bit(mode_abs_x(addr));
        cycles(4 + page_crossed(addr, x & 0xff));
        return addr + 2;
    }

    @Override
    public int bit_dp(int addr)
    {
        bit(mode_dp(addr));
        cycles(3);
        return addr + 1;
    }

    @Override
    public int bit_dp_x(int addr)
    {
        bit(mode_dp_x(addr));
        cycles(4);
        return addr + 1;
    }

    @Override
    public int bit_imm(int addr)
    {
        int value = mem.read8(addr);;
        p &= ~z_mask;
        if ((value & a) == 0)
            p |= z_mask;
        cycles(2);
        return addr + 1;
    }

    @Override
    public int bmi(int addr)
    {
        addr += 1;
        if ((p & n_mask) != 0)
        {
            int imm = (byte)mem.read8(addr - 1);
            addr += imm;
            cycles(1 + page_crossed(addr, -imm));
        }
        cycles(2);
        return addr;
    }

    @Override
    public int bne(int addr)
    {
        addr += 1;
        if ((p & z_mask) == 0)
        {
            int imm = (byte)mem.read8(addr - 1);
            addr += imm;
            cycles(1 + page_crossed(addr, -imm));
        }
        cycles(2);
        return addr;
    }

    @Override
    public int bpl(int addr)
    {
        addr += 1;
        if ((p & n_mask) == 0)
        {
            int imm = (byte)mem.read8(addr - 1);
            addr += imm;
            cycles(1 + page_crossed(addr, -imm));
        }
        cycles(2);
        return addr;
    }

    @Override
    public int bra(int addr)
    {
        int imm = (byte)mem.read8(addr);
        addr += imm + 1;
        cycles(3 + page_crossed(addr, -imm));
        return addr;
    }

    @Override
    public int brk(int addr)
    {
        mem.write16(push16(), addr + 1);
        mem.write8(push(), p | b_mask);
        cycles(7);
        return mem.read16(0xfffe);
    }

    @Override
    public int brl(int addr)
    {
        return unimplemented_2_2(addr);
    }

    @Override
    public int bvc(int addr)
    {
        addr += 1;
        if ((p & v_mask) == 0)
        {
            int imm = (byte)mem.read8(addr - 1);
            addr += imm;
            cycles(1 + page_crossed(addr, -imm));
        }
        cycles(2);
        return addr;
    }

    @Override
    public int bvs(int addr)
    {
        addr += 1;
        if ((p & v_mask) != 0)
        {
            int imm = (byte)mem.read8(addr - 1);
            addr += imm;
            cycles(1 + page_crossed(addr, -imm));
        }
        cycles(2);
        return addr;
    }

    @Override
    public int clc(int addr)
    {
        p &= ~c_mask;
        cycles(2);
        return addr;
    }
    
    @Override
    public int cld(int addr)
    {
        p &= ~d_mask;
        cycles(2);
        return addr;
    }
    
    @Override
    public int cli(int addr)
    {
        p &= ~i_mask;
        update_attention();
        cycles(2);
        return addr;
    }
    
    @Override
    public int clv(int addr)
    {
        p &= ~v_mask;
        cycles(2);
        return addr;
    }
    
    @Override
    public int cmp_abs(int addr)
    {
        cmp(a, mode_abs(addr));
        cycles(4);
        return addr + 2;
    }

    @Override
    public int cmp_abs_x(int addr)
    {
        cmp(a, mode_abs_x(addr));
        cycles(4  + page_crossed(addr, x & 0xff));
        return addr + 2;
    }

    @Override
    public int cmp_abs_y(int addr)
    {
        cmp(a, mode_abs_y(addr));
        cycles(4 + page_crossed(addr, y & 0xff));
        return addr + 2;
    }

    @Override
    public int cmp_abslong(int addr)
    {
        return bbs(addr, 4);
    }
    
    @Override
    public int cmp_abslong_x(int addr)
    {
        return bbs(addr, 5);
    }
    
    @Override
    public int cmp_dp(int addr)
    {
        cmp(a, mode_dp(addr));
        cycles(3);
        return addr + 1;    
    }

    @Override
    public int cmp_dp__(int addr)
    {
        cmp(a, mode_dp__(addr));
        cycles(5);
        return addr + 1;
    }

    @Override
    public int cmp_dp_x(int addr)
    {
        cmp(a, mode_dp_x(addr));
        cycles(4);
        return addr + 1;    
    }

    @Override
    public int cmp_dp_x__(int addr)
    {
        cmp(a, mode_dp_x__(addr));
        cycles(6);
        return addr + 1;
    }

    @Override
    public int cmp_dp___y(int addr)
    {
        cmp(a, mode_dp___y(addr));
        cycles(5 + page_crossed_zindirect(addr, y & 0xff));
        return addr + 1;
    }

    @Override
    public int cmp_dp__long(int addr)
    {
        return smb(addr, 4);
    }

    @Override
    public int cmp_dp__long_y(int addr)
    {
        return smb(addr, 5);
    }

    @Override
    public int cmp_imm(int addr)
    {
        cmp(a, mem.read8(addr));
        cycles(2);
        return addr + 1;
    }

    @Override
    public int cmp_sr(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int cmp_sr__y(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int cop(int addr)
    {
        return unimplemented_2_2(addr);
    }

    @Override
    public int cpx_abs(int addr)
    {
        cmp(x, mode_abs(addr));
        cycles(4);
        return addr + 2;
    }

    @Override
    public int cpx_dp(int addr)
    {
        cmp(x, mode_dp(addr));
        cycles(3);
        return addr + 1;    
    }

    @Override
    public int cpx_imm(int addr)
    {
        cmp(x, mem.read8(addr));
        cycles(2);
        return addr + 1;
    }

    @Override
    public int cpy_abs(int addr)
    {
        cmp(y, mode_abs(addr));
        cycles(4);
        return addr + 2;
    }

    @Override
    public int cpy_dp(int addr)
    {
        cmp(y, mode_dp(addr));
        cycles(3);
        return addr + 1;    
    }

    @Override
    public int cpy_imm(int addr)
    {
        cmp(y, mem.read8(addr));
        cycles(2);
        return addr + 1;
    }

    @Override
    public int dec_abs(int addr)
    {
        dec_ea(mode_abs_ea(addr));
        cycles(6);
        return addr + 2;    
    }

    @Override
    public int dec_abs_x(int addr)
    {
        dec_ea(mode_abs_x_ea(addr));
        cycles(6 + page_crossed(addr, x & 0xff));
        return addr + 2;    
    }

    @Override
    public int dec_acc(int addr)
    {
        --a;
        update_flags_n_z(a);
        cycles(2);
        return addr;
    }

    @Override
    public int dec_dp(int addr)
    {
        dec_ea(mode_dp_ea(addr));
        cycles(5);
        return addr + 1;    
    }

    @Override
    public int dec_dp_x(int addr)
    {
        dec_ea(mode_dp_x_ea(addr));
        cycles(6);
        return addr + 1;    
    }

    @Override
    public int dex(int addr)
    {
        --x;
        update_flags_n_z(x);
        cycles(2);
        return addr;
    }
    
    @Override
    public int dey(int addr)
    {
        --y;
        update_flags_n_z(y);
        cycles(2);
        return addr;
    }
    
    @Override
    public int eor_abs(int addr)
    {
        a ^= mode_abs(addr);
        update_flags_n_z(a);
        cycles(4);
        return addr + 2;    
    }

    @Override
    public int eor_abs_x(int addr)
    {
        a ^= mode_abs_x(addr);
        update_flags_n_z(a);
        cycles(4 + page_crossed(addr, x & 0xff));
        return addr + 2;
    }

    @Override
    public int eor_abs_y(int addr)
    {
        a ^= mode_abs_y(addr);
        update_flags_n_z(a);
        cycles(4 + page_crossed(addr, y & 0xff));
        return addr + 2;
    }

    @Override
    public int eor_abslong(int addr)
    {
        return bbr(addr, 4);
    }
    
    @Override
    public int eor_abslong_x(int addr)
    {
        return bbr(addr, 5);
    }
    
    @Override
    public int eor_dp(int addr)
    {
        a ^= mode_dp(addr);
        update_flags_n_z(a);
        cycles(3);
        return addr + 1;    
    }

    @Override
    public int eor_dp__(int addr)
    {
        a ^= mode_dp__(addr);
        update_flags_n_z(a);
        cycles(5);
        return addr + 1;
    }

    @Override
    public int eor_dp_x(int addr)
    {
        a ^= mode_dp_x(addr);
        update_flags_n_z(a);
        cycles(4);
        return addr + 1;    
    }

    @Override
    public int eor_dp_x__(int addr)
    {
        a ^= mode_dp_x__(addr);
        update_flags_n_z(a);
        cycles(6);
        return addr + 1;
    }

    @Override
    public int eor_dp___y(int addr)
    {
        a ^= mode_dp___y(addr);
        update_flags_n_z(a);
        cycles(5 + page_crossed_zindirect(addr, y & 0xff));
        return addr + 1;
    }

    @Override
    public int eor_dp__long(int addr)
    {
        return rmb(addr, 4);
    }

    @Override
    public int eor_dp__long_y(int addr)
    {
        return rmb(addr, 5);
    }

    @Override
    public int eor_imm(int addr)
    {
        a ^= mem.read8(addr);
        update_flags_n_z(a);
        cycles(2);
        return addr + 1;
    }

    @Override
    public int eor_sr(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int eor_sr__y(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int inc_abs(int addr)
    {
        inc_ea(mode_abs_ea(addr));
        cycles(6);
        return addr + 2;    
    }

    @Override
    public int inc_abs_x(int addr)
    {
        inc_ea(mode_abs_x_ea(addr));
        cycles(6 + page_crossed(addr, x & 0xff));
        return addr + 2;    
    }

    @Override
    public int inc_acc(int addr)
    {
        ++a;
        update_flags_n_z(a);
        cycles(2);
        return addr;
    }

    @Override
    public int inc_dp(int addr)
    {
        inc_ea(mode_dp_ea(addr));
        cycles(5);
        return addr + 1;    
    }

    @Override
    public int inc_dp_x(int addr)
    {
        inc_ea(mode_dp_x_ea(addr));
        cycles(6);
        return addr + 1;    
    }

    @Override
    public int inx(int addr)
    {
        ++x;
        update_flags_n_z(x);
        cycles(2);
        return addr;
    }
    
    @Override
    public int iny(int addr)
    {
        ++y;
        update_flags_n_z(y);
        cycles(2);
        return addr;
    }
    
    @Override
    public int jml(int addr)
    {
        return unimplemented_3_8(addr);
    }

    @Override
    public int jml__(int addr)
    {
        return unimplemented_3_4(addr);
    }

    @Override
    public int jmp_abs(int addr)
    {
        int new_addr = mem.read16(addr);
        cycles(3);
        return new_addr;
    }
    
    @Override
    public int jmp_abs__(int addr)
    {
        int new_addr = mem.read16(mem.read16(addr));
        cycles(6);
        return new_addr;
    }
    
    @Override
    public int jmp_abs_x__(int addr)
    {
        int new_addr = mem.read16(mem.read16(addr) + x);
        cycles(6);
        return new_addr;
    }

    @Override
    public int jsl(int addr)
    {
        return unimplemented_2_2(addr);
    }

    @Override
    public int jsr_abs(int addr)
    {
        int new_addr = mem.read16(addr);
        addr = addr + 1;
        mem.write16(push16(), addr);
        cycles(6);
        return new_addr;
    }
    
    @Override
    public int jsr_abs_x__(int addr)
    {
        return unimplemented_3_4(addr);
    }

    @Override
    public int lda_abs(int addr)
    {
        int abs_addr = mem.read16(addr);
        a = (byte)mem.read8(abs_addr);
        update_flags_n_z(a);
        cycles(4);
        return addr + 2;
    }

    @Override
    public int lda_abs_x(int addr)
    {
        a = (byte)mode_abs_x(addr);
        update_flags_n_z(a);
        cycles(4 + page_crossed(addr, x & 0xff));
        return addr + 2;
    }

    @Override
    public int lda_abs_y(int addr)
    {
        a = (byte)mode_abs_y(addr);
        update_flags_n_z(a);
        cycles(4 + page_crossed(addr, y & 0xff));
        return addr + 2;
    }

    @Override
    public int lda_abslong(int addr)
    {
        return bbs(addr, 2);
    }
    
    @Override
    public int lda_abslong_x(int addr)
    {
        return bbs(addr, 3);
    }
    
    @Override
    public int lda_dp(int addr)
    {
        a = (byte)mode_dp(addr);
        update_flags_n_z(a);
        cycles(3);
        return addr + 1;    
    }

    @Override
    public int lda_dp__(int addr)
    {
        a = (byte)mode_dp__(addr);
        update_flags_n_z(a);
        cycles(5);
        return addr + 1;
    }

    @Override
    public int lda_dp_x(int addr)
    {
        a = (byte)mode_dp_x(addr);
        update_flags_n_z(a);
        cycles(4);
        return addr + 1;    
    }

    @Override
    public int lda_dp_x__(int addr)
    {
        a = (byte)mode_dp_x__(addr);
        update_flags_n_z(a);
        cycles(6);
        return addr + 1;
    }

    @Override
    public int lda_dp___y(int addr)
    {
        a = (byte)mode_dp___y(addr);
        update_flags_n_z(a);
        cycles(5 + page_crossed_zindirect(addr, y & 0xff));
        return addr + 1;
    }

    @Override
    public int lda_dp__long(int addr)
    {
        return smb(addr, 2);
    }

    @Override
    public int lda_dp__long_y(int addr)
    {
        return smb(addr, 3);
    }

    @Override
    public int lda_imm(int addr)
    {
        a = (byte)mem.read8(addr);
        update_flags_n_z(a);
        cycles(2);
        return addr + 1;
    }

    @Override
    public int lda_sr(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int lda_sr__y(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int ldx_abs(int addr)
    {
        int abs_addr = mem.read16(addr);
        x = (byte)mem.read8(abs_addr);
        update_flags_n_z(x);
        cycles(4);
        return addr + 2;
    }

    @Override
    public int ldx_abs_y(int addr)
    {
        x = (byte)mode_abs_y(addr);
        update_flags_n_z(x);
        cycles(4 + page_crossed(addr, y & 0xff));
        return addr + 2;
    }

    @Override
    public int ldx_dp(int addr)
    {
        x = (byte)mode_dp(addr);
        update_flags_n_z(x);
        cycles(3);
        return addr + 1;    
    }

    @Override
    public int ldx_dp_y(int addr)
    {
        x = (byte)mode_dp_y(addr);
        update_flags_n_z(x);
        cycles(4);
        return addr + 1;    
    }

    @Override
    public int ldx_imm(int addr)
    {
        x = (byte)mem.read8(addr);
        update_flags_n_z(x);
        cycles(2);
        return addr + 1;
    }

    @Override
    public int ldy_abs(int addr)
    {
        int abs_addr = mem.read16(addr);
        y = (byte)mem.read8(abs_addr);
        update_flags_n_z(y);
        cycles(4);
        return addr + 2;
    }

    @Override
    public int ldy_abs_x(int addr)
    {
        y = (byte)mode_abs_x(addr);
        update_flags_n_z(y);
        cycles(4 + page_crossed(addr, x & 0xff));
        return addr + 2;
    }

    @Override
    public int ldy_dp(int addr)
    {
        y = (byte)mode_dp(addr);
        update_flags_n_z(y);
        cycles(3);
        return addr + 1;    
    }

    @Override
    public int ldy_dp_x(int addr)
    {
        y = (byte)mode_dp_x(addr);
        update_flags_n_z(y);
        cycles(4);
        return addr + 1;    
    }

    @Override
    public int ldy_imm(int addr)
    {
        y = (byte)mem.read8(addr);
        update_flags_n_z(y);
        cycles(2);
        return addr + 1;
    }

    @Override
    public int lsr_abs(int addr)
    {
        lsr_ea(mode_abs_ea(addr));
        cycles(6);
        return addr + 2;    
    }

    @Override
    public int lsr_abs_x(int addr)
    {
        lsr_ea(mode_abs_x_ea(addr));
        cycles(6 + page_crossed(addr, x & 0xff));
        return addr + 2;    
    }

    @Override
    public int lsr_acc(int addr)
    {
        p &= ~c_mask;
        p |= (a << c_bit) & c_mask;
        a = (byte)((a & 0xff) >> 1);
        update_flags_n_z(a);
        cycles(2);
        return addr;
    }

    @Override
    public int lsr_dp(int addr)
    {
        lsr_ea(mode_dp_ea(addr));
        cycles(5);
        return addr + 1;    
    }

    @Override
    public int lsr_dp_x(int addr)
    {
        lsr_ea(mode_dp_x_ea(addr));
        cycles(6);
        return addr + 1;    
    }

    @Override
    public int mvn(int addr)
    {
        return unimplemented_2_4(addr);
    }

    @Override
    public int mvp(int addr)
    {
        return unimplemented_2_3(addr);
    }

    @Override
    public int nop(int addr)
    {
        cycles(2);
        return addr;
    }
    
    @Override
    public int ora_abs(int addr)
    {
        a |= mode_abs(addr);
        update_flags_n_z(a);
        cycles(4);
        return addr + 2;    
    }

    @Override
    public int ora_abs_x(int addr)
    {
        a |= mode_abs_x(addr);
        update_flags_n_z(a);
        cycles(4 + page_crossed(addr, x & 0xff));
        return addr + 2;
    }

    @Override
    public int ora_abs_y(int addr)
    {
        a |= mode_abs_y(addr);
        update_flags_n_z(a);
        cycles(4 + page_crossed(addr, y & 0xff));
        return addr + 2;
    }

    @Override
    public int ora_abslong(int addr)
    {
        return bbr(addr, 0);
    }
    
    @Override
    public int ora_abslong_x(int addr)
    {
        return bbr(addr, 1);
    }
    
    @Override
    public int ora_dp(int addr)
    {
        a |= mode_dp(addr);
        update_flags_n_z(a);
        cycles(3);
        return addr + 1;    
    }

    @Override
    public int ora_dp__(int addr)
    {
        a |= mode_dp__(addr);
        update_flags_n_z(a);
        cycles(5);
        return addr + 1;
    }

    @Override
    public int ora_dp_x(int addr)
    {
        a |= mode_dp_x(addr);
        update_flags_n_z(a);
        cycles(4);
        return addr + 1;    
    }

    @Override
    public int ora_dp_x__(int addr)
    {
        a |= mode_dp_x__(addr);
        update_flags_n_z(a);
        cycles(6);
        return addr + 1;
    }

    @Override
    public int ora_dp___y(int addr)
    {
        a |= mode_dp___y(addr);
        update_flags_n_z(a);
        cycles(5 + page_crossed_zindirect(addr, y & 0xff));
        return addr + 1;
    }

    @Override
    public int ora_dp__long(int addr)
    {
        return rmb(addr, 0);
    }

    @Override
    public int ora_dp__long_y(int addr)
    {
        return rmb(addr, 1);
    }

    @Override
    public int ora_imm(int addr)
    {
        a |= mem.read8(addr);
        update_flags_n_z(a);
        cycles(2);
        return addr + 1;
    }

    @Override
    public int ora_sr(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int ora_sr__y(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int pea(int addr)
    {
        return unimplemented_2_4(addr);
    }

    @Override
    public int pei(int addr)
    {
        return unimplemented_2_4(addr);
    }

    @Override
    public int per(int addr)
    {
        return unimplemented_2_2(addr);
    }

    @Override
    public int pha(int addr)
    {
        mem.write8(push(), a);
        cycles(3);
        return addr;
    }
    
    @Override
    public int phb(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int phd(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int phk(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int php(int addr)
    {
        mem.write8(push(), p);
        cycles(3);
        return addr;
    }
    
    @Override
    public int phx(int addr)
    {
        mem.write8(push(), x);
        cycles(3);
        return addr;
    }
    
    @Override
    public int phy(int addr)
    {
        mem.write8(push(), y);
        cycles(3);
        return addr;
    }
    
    @Override
    public int pla(int addr)
    {
        a = (byte)mem.read8(pull());
        update_flags_n_z(a);
        cycles(4);
        return addr;
    }
    
    @Override
    public int plb(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int pld(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int plp(int addr)
    {
// Unused bit (5) appears to be always high in a real 65C02.
// Bit b (4) appears to be unchanged by a PLP instruction in a real 65C02.
// I did not find this documented though.
        p &= 0x30;
        p |= (byte)(mem.read8(pull()) & 0xcf);
        cycles(4);
        return addr;
    }
    
    @Override
    public int plx(int addr)
    {
        x = (byte)mem.read8(pull());
        update_flags_n_z(x);
        cycles(4);
        return addr;
    }
    
    @Override
    public int ply(int addr)
    {
        y = (byte)mem.read8(pull());
        update_flags_n_z(y);
        cycles(4);
        return addr;
    }
    
    @Override
    public int rep(int addr)
    {
        return unimplemented_2_2(addr);
    }

    @Override
    public int rol_abs(int addr)
    {
        rol_ea(mode_abs_ea(addr));
        cycles(6);
        return addr + 2;    
    }

    @Override
    public int rol_abs_x(int addr)
    {
        rol_ea(mode_abs_x_ea(addr));
        cycles(6 + page_crossed(addr, x & 0xff));
        return addr + 2;    
    }

    @Override
    public int rol_acc(int addr)
    {
        int rbit = (p & c_mask) >> c_bit;
        p &= ~c_mask;
        p |= (a & 0x80) >> (7 - c_bit);
        a <<= 1;
        a |= rbit;
        update_flags_n_z(a);
        cycles(2);
        return addr;
    }

    @Override
    public int rol_dp(int addr)
    {
        rol_ea(mode_dp_ea(addr));
        cycles(5);
        return addr + 1;    
    }

    @Override
    public int rol_dp_x(int addr)
    {
        rol_ea(mode_dp_x_ea(addr));
        cycles(6);
        return addr + 1;    
    }

    @Override
    public int ror_abs(int addr)
    {
        ror_ea(mode_abs_ea(addr));
        cycles(6);
        return addr + 2;    
    }

    @Override
    public int ror_abs_x(int addr)
    {
        ror_ea(mode_abs_x_ea(addr));
        cycles(6 + page_crossed(addr, x & 0xff));
        return addr + 2;    
    }

    @Override
    public int ror_acc(int addr)
    {
        int lbit = (p & c_mask) << (7 - c_bit);
        p &= ~c_mask;
        p |= (a & 1) << c_bit;
        a = (byte)((a & 0xff) >> 1);
        a |= lbit;
        update_flags_n_z(a);
        cycles(2);
        return addr;
    }

    @Override
    public int ror_dp(int addr)
    {
        ror_ea(mode_dp_ea(addr));
        cycles(5);
        return addr + 1;    
    }

    @Override
    public int ror_dp_x(int addr)
    {
        ror_ea(mode_dp_x_ea(addr));
        cycles(6);
        return addr + 1;    
    }

    @Override
    public int rti(int addr)
    {
        p = (byte)mem.read8(pull());
        p |= u_mask | b_mask;
        int new_addr = mem.read8(pull());
        new_addr |= mem.read8(pull()) << 8;
        cycles(6);
        return new_addr;
    }
    
    @Override
    public int rtl(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int rts(int addr)
    {
        int new_addr = mem.read16(pull16());
        new_addr += 1;
        cycles(6);
        return new_addr;
    }
    
    @Override
    public int sbc_abs(int addr)
    {
        a = (byte)sub(a, mode_abs(addr));
        cycles(4);
        cycles(decimal_set());
        return addr + 2;    
    }

    @Override
    public int sbc_abs_x(int addr)
    {
        a = (byte)sub(a, mode_abs_x(addr));
        cycles(4 + decimal_set() + page_crossed(addr, x & 0xff));
        return addr + 2;
    }

    @Override
    public int sbc_abs_y(int addr)
    {
        a = (byte)sub(a, mode_abs_y(addr));
        cycles(4 + decimal_set() + page_crossed(addr, y & 0xff));
        return addr + 2;
    }

    @Override
    public int sbc_abslong(int addr)
    {
        return bbs(addr, 6);
    }
    
    @Override
    public int sbc_abslong_x(int addr)
    {
        return bbs(addr, 7);
    }
    
    @Override
    public int sbc_dp(int addr)
    {
        a = (byte)sub(a, mode_dp(addr));
        cycles(3 + decimal_set());
        return addr + 1;    
    }

    @Override
    public int sbc_dp__(int addr)
    {
        a = (byte)sub(a, mode_dp__(addr));
        cycles(5 + decimal_set());
        return addr + 1;
    }

    @Override
    public int sbc_dp_x(int addr)
    {
        a = (byte)sub(a, mode_dp_x(addr));
        cycles(4 + decimal_set());
        return addr + 1;    
    }

    @Override
    public int sbc_dp_x__(int addr)
    {
        a = (byte)sub(a, mode_dp_x__(addr));
        cycles(6 + decimal_set());
        return addr + 1;
    }

    @Override
    public int sbc_dp___y(int addr)
    {
        a = (byte)sub(a, mode_dp___y(addr));
        cycles(5 + decimal_set() + page_crossed_zindirect(addr, y & 0xff));
        return addr + 1;
    }

    @Override
    public int sbc_dp__long(int addr)
    {
        return smb(addr, 6);
    }

    @Override
    public int sbc_dp__long_y(int addr)
    {
        return smb(addr, 7);
    }

    @Override
    public int sbc_imm(int addr)
    {
        a = (byte)sub(a, mem.read8(addr));
        cycles(2 + decimal_set());
        return addr + 1;
    }

    @Override
    public int sbc_sr(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int sbc_sr__y(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int sec(int addr)
    {
        p |= c_mask;
        cycles(2);
        return addr;
    }
    
    @Override
    public int sed(int addr)
    {
        p |= d_mask;
        cycles(2);
        return addr;
    }
    
    @Override
    public int sei(int addr)
    {
        p |= i_mask;
        update_attention();
        cycles(2);
        return addr;
    }
    
    @Override
    public int sep(int addr)
    {
        return unimplemented_2_2(addr);
    }

    @Override
    public int sta_abs(int addr)
    {
        int dst_addr = mode_abs_ea(addr);
        mem.write8(dst_addr, a);
        cycles(4);
        return addr + 2;
    }
    
    @Override
    public int sta_abs_x(int addr)
    {
        int dst_addr = mode_abs_x_ea(addr);
        mem.write8(dst_addr, a);
        cycles(5);
        return addr + 2;
    }
    
    @Override
    public int sta_abs_y(int addr)
    {
        int dst_addr = mode_abs_y_ea(addr);
        mem.write8(dst_addr, a);
        cycles(5);
        return addr + 2;
    }
    
    @Override
    public int sta_abslong(int addr)
    {
        return bbs(addr, 0);
    }
    
    @Override
    public int sta_abslong_x(int addr)
    {
        return bbs(addr, 1);
    }
    
    @Override
    public int sta_dp(int addr)
    {
        int dst_addr = mode_dp_ea(addr);
        mem.write8(dst_addr, a);
        cycles(3);
        return addr + 1;    
    }

    @Override
    public int sta_dp__(int addr)
    {
        int dst_addr = mode_dp___ea(addr);
        mem.write8(dst_addr, a);
        cycles(5);
        return addr + 1;
    }

    @Override
    public int sta_dp_x(int addr)
    {
        int dst_addr = mode_dp_x_ea(addr);
        mem.write8(dst_addr, a);
        cycles(4);
        return addr + 1;    
    }

    @Override
    public int sta_dp_x__(int addr)
    {
        int dst_addr = mode_dp_x___ea(addr);
        mem.write8(dst_addr, a);
        cycles(6);
        return addr + 1;
    }

    @Override
    public int sta_dp___y(int addr)
    {
        int dst_addr = mode_dp___y_ea(addr);
        mem.write8(dst_addr, a);
        cycles(6);
        return addr + 1;
    }

    @Override
    public int sta_dp__long(int addr)
    {
        return smb(addr, 0);
    }

    @Override
    public int sta_dp__long_y(int addr)
    {
        return smb(addr, 1);
    }

    @Override
    public int sta_sr(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int sta_sr__y(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int stp(int addr)
    {
        if (pausable)
        {
            cycles(3);
            while (vdd && !resb_sequence)
            {
                try
                {
                    Thread.sleep(1);
                }
                catch(InterruptedException e)
                {
                    vdd = false;
                    fail_msg = e.getMessage();
                    failed = true;
                }
            }
            return addr;
        }
        else
            return unimplemented_1_1(addr);
    }
    
    @Override
    public int stx_abs(int addr)
    {
        int dst_addr = mode_abs_ea(addr);
        mem.write8(dst_addr, x);
        cycles(4);
        return addr + 2;
    }
    
    @Override
    public int stx_dp(int addr)
    {
        int dst_addr = mode_dp_ea(addr);
        mem.write8(dst_addr, x);
        cycles(3);
        return addr + 1;    
    }

    @Override
    public int stx_dp_y(int addr)
    {
        int dst_addr = mode_dp_y_ea(addr);
        mem.write8(dst_addr, x);
        cycles(4);
        return addr + 1;    
    }

    @Override
    public int sty_abs(int addr)
    {
        int dst_addr = mode_abs_ea(addr);
        mem.write8(dst_addr, y);
        cycles(4);
        return addr + 2;
    }
    
    @Override
    public int sty_dp(int addr)
    {
        int dst_addr = mode_dp_ea(addr);
        mem.write8(dst_addr, y);
        cycles(3);
        return addr + 1;    
    }

    @Override
    public int sty_dp_x(int addr)
    {
        int dst_addr = mode_dp_x_ea(addr);
        mem.write8(dst_addr, y);
        cycles(4);
        return addr + 1;    
    }

    @Override
    public int stz_abs(int addr)
    {
        int dst_addr = mode_abs_ea(addr);
        mem.write8(dst_addr, 0);
        cycles(4);
        return addr + 2;
    }
    
    @Override
    public int stz_abs_x(int addr)
    {
        int dst_addr = mode_abs_x_ea(addr);
        mem.write8(dst_addr, 0);
        cycles(5);
        return addr + 2;
    }
    
    @Override
    public int stz_dp(int addr)
    {
        int dst_addr = mode_dp_ea(addr);
        mem.write8(dst_addr, 0);
        cycles(3);
        return addr + 1;    
    }

    @Override
    public int stz_dp_x(int addr)
    {
        int dst_addr = mode_dp_x_ea(addr);
        mem.write8(dst_addr, 0);
        cycles(4);
        return addr + 1;    
    }

    @Override
    public int tax(int addr)
    {
        x = a;
        update_flags_n_z(x);
        cycles(2);
        return addr;
    }
    
    @Override
    public int tay(int addr)
    {
        y = a;
        update_flags_n_z(y);
        cycles(2);
        return addr;
    }
    
    @Override
    public int tcd(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int tcs(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int tdc(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int trb_abs(int addr)
    {
        int dst_addr = mode_abs_ea(addr);
        int value = mem.read8(dst_addr);
        p &= ~z_mask;
        if ((value & a) == 0)
            p |= z_mask;
        mem.write8(dst_addr, value & ~a);
        cycles(6);
        return addr + 2;
    }
    
    @Override
    public int trb_dp(int addr)
    {
        int dst_addr = mode_dp_ea(addr);
        int value = mem.read8(dst_addr);
        p &= ~z_mask;
        if ((value & a) == 0)
            p |= z_mask;
        mem.write8(dst_addr, value & ~a);
        cycles(5);
        return addr + 1;    
    }

    @Override
    public int tsb_abs(int addr)
    {
        int dst_addr = mode_abs_ea(addr);
        int value = mem.read8(dst_addr);
        p &= ~z_mask;
        if ((value & a) == 0)
            p |= z_mask;
        mem.write8(dst_addr, value | a);
        cycles(6);
        return addr + 2;
    }
    
    @Override
    public int tsb_dp(int addr)
    {
        int dst_addr = mode_dp_ea(addr);
        int value = mem.read8(dst_addr);
        p &= ~z_mask;
        if ((value & a) == 0)
            p |= z_mask;
        mem.write8(dst_addr, value | a);
        cycles(5);
        return addr + 1;    
    }

    @Override
    public int tsc(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int tsx(int addr)
    {
        x = s;
        update_flags_n_z(x);
        cycles(2);
        return addr;
    }
    
    @Override
    public int txa(int addr)
    {
        a = x;
        update_flags_n_z(a);
        cycles(2);
        return addr;
    }
    
    @Override
    public int txs(int addr)
    {
        s = x;
        cycles(2);
        return addr;
    }
    
    @Override
    public int txy(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int tya(int addr)
    {
        a = y;
        update_flags_n_z(a);
        cycles(2);
        return addr;
    }
    
    @Override
    public int tyx(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int wai(int addr)
    {
        if (pausable)
        {
            cycles(3);
            boolean interrupted = resb_sequence || nmi_sequence || irqline_low;
            while (vdd && !interrupted)
            {
                try
                {
                    Thread.sleep(1);
                }
                catch(InterruptedException e)
                {
                    vdd = false;
                    fail_msg = e.getMessage();
                    failed = true;
                }
                interrupted = resb_sequence || nmi_sequence || irqline_low;
            };
            return addr;
        }
        else
            return unimplemented_1_1(addr);
    }
    
    @Override
    public int wdm(int addr)
    {
        return unimplemented_2_2(addr);
    }

    @Override
    public int xba(int addr)
    {
        return unimplemented_1_1(addr);
    }

    @Override
    public int xce(int addr)
    {
        return unimplemented_1_1(addr);
    }

    static private int c_bit = 0;
    static private int z_bit = 1;
    static private int i_bit = 2;
    static private int d_bit = 3;
    static private int b_bit = 4;
    static private int u_bit = 5;
    static private int v_bit = 6; 
    static private int n_bit = 7; 
    
    static private int c_mask = 1 << c_bit;
    static private int z_mask = 1 << z_bit;
    static private int i_mask = 1 << i_bit;
    static private int d_mask = 1 << d_bit;
    static private int b_mask = 1 << b_bit;
    static private int u_mask = 1 << u_bit;
    static private int v_mask = 1 << v_bit;
    static private int n_mask = 1 << n_bit;

    private byte a;
    private byte x;
    private byte y;
    private byte p;
    private byte s;
    private int pc;
    
    private int period;
    private long last_time = System.nanoTime();
    Power power = null;
    private volatile boolean vdd = false;
    private volatile boolean attention_needed = false;
    private volatile boolean resb_sequence = false;
    private volatile boolean resb_high = false;
    private volatile boolean nmi_sequence = false;
    private volatile boolean nmi_high = true;
    private volatile boolean irqline_low = false;
    
    private boolean fail_on_unimplemented = false;
    private volatile boolean failed = false;
    private String fail_msg = "";
    private boolean rockwell = false;
    private boolean pausable = false;
    
    Parser parser;
    private Mem mem;
}
