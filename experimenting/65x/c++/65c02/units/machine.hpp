// Test machine
//
// Created by Alejandro Castro García on 3 May 2023
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


#ifndef w65c02_machine_hpp
#define w65c02_machine_hpp

#include "cpu65c02.hpp"
#include "65c02.hpp"
#include "ui.hpp"

namespace w65c02
{

using Mem_le = faulib::format::Memory_le<Address, Byte, Word, Long, Quad>;


class Mem_le_plain: public Mem_le
{
public:
    Byte read8(Address addr) override
    {
        return bytes[addr];
    }
    void write8(Address addr, Byte v) override
    {
        bytes[addr] = v;
    }
    
    void copybytes(unsigned char* dst, size_t size) override
    {
        size = (size <= 65536 ? size : 65536);
        memcpy(dst, bytes.data(), size);
    }

private:
    vector<Byte> bytes = vector<Byte>(65536);
};

class Test_machine;

class Mem_test: public Mem_le_plain
{
    static constexpr Address action_mask =  0xf000;
    static constexpr Address action_area =  0xe000;
public:
    static constexpr Address poweroff =  action_area;
    static constexpr Address report_brk = action_area + 1;
    static constexpr Address report_res = action_area + 2;
    static constexpr Address report_irq = action_area + 3;
    static constexpr Address report_nmi = action_area + 4;
    static constexpr Address interrupt_done = action_area + 5;

    Mem_test(Test_machine& m):
    machine(m)
    {
    }
    
    void write8(Address addr, Byte v) override
    {
        Mem_le_plain::write8(addr, v);
        if ((addr & action_mask) == action_area)
            action(addr, v);
    }
private:
    Test_machine& machine;
    
    void action(Address addr, Byte v);
};

class Test_machine_error: public runtime_error
{
public:
    Test_machine_error(const string& what_arg): runtime_error(what_arg)
    {}
};

class Test_machine
{
public:
    Test_machine
        (string test_path, bool fail, bool rockwell, bool pausable, bool ui):
    mem(*this),
    priv_cpu(mem),
    test_path(test_path),
    on(false),
    ui(*this)
    {
        if (fail)
            priv_cpu.fail_on_unimplemented();
        else
            priv_cpu.nop_on_unimplemented();
        priv_cpu.rockwell_additions(rockwell);
        priv_cpu.pause_additions(pausable);
        this->ui.listen(ui);
    }
    
    void turn_on();
    
    void power(bool on)
    {
        this->on = on;
    }
    
    void irq_clear()
    {
        cpu().irq_high();
    }

    Cpu65c02& cpu()
    {
        return priv_cpu;
    }
    
private:
    Mem_test mem;
    Cpu65c02 priv_cpu;
    string test_path;
    Word result_size = 0;
    Address result_address = 0;
    atomic<bool> on;
    User_interface ui;
    
    void off();
    void compare();
};

} //namespace w65c02
#endif /* w65c02_machine_hpp */
