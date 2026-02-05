// sxbenv.hpp
// Minimal environment where 65C02 test programs will run: interface
//
// Created by Alejandro Castro García on 3 September 2021
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


#ifndef w65c02sxbenv_hpp
#define w65c02sxbenv_hpp

#include <nexo/io/io.hpp>
#include "sxb.hpp"

namespace w65c02
{

class Error_sxbenv_code_misplaced: public logic_error
{
public:
    Error_sxbenv_code_misplaced():
    logic_error("w65c02 Error_sxbenv_code_misplaced") {}
};

enum class Xcs
{
    zero,
    one,
    two,
    three
};

Address send_call(Sxb& sxb, Address pc, Address called_addr);

Address send_jump(Sxb& sxb, Address pc, Address called_addr);

Address send_break(Sxb& sxb, Address pc, Byte arg);

Address send_led_routine(Sxb &sxb, Address pc, Xcs chip);

Address send_break_after_repeated_call
            (Sxb& sxb, Address pc, Xcs, unsigned count, Address called_addr);

vector<Address16> get_vectors(Sxb& sxb);

void send_vectors(Sxb& sxb, vector<Address16> vector_addr);

Address send_infinite_loop(Sxb& sxb, Address pc);

Address send_rti(Sxb& sxb, Address pc);

Address interrupt_routines(Sxb& sxb, Address pc);

Address16 irq_brk_routine(Sxb& sxb, Address& pc);

Address bridge_led(Sxb& sxb, Address prog_addr);

Address bridge_infinite(Sxb& sxb, Address prog_addr);

Address bridge_interrupts(Sxb& sxb, Address prog_addr);

Address bridge_plain(Sxb& sxb, Address prog_addr);

inline Address bridge(Sxb& sxb, Address prog_addr)
{
// After the uploaded program terminates, we need some way to return control to
// wdcmon, so we can keep communicating and download the result.
// It seems that inserting a breakpoint does the trick
// That's what the bridge_* functions do, except bridge_infinite, which instead
// executes an infinite loop after running the program.
// - bridge_plain: just runs the program, without installing interrupt handlers.
// - bridge_interrupts: installs interrupt handlers and runs the program.
// - bridge_led: runs the program, then plays with leds a bit before returning.
// - bridge_infinite: installs handlers, runs the program and loops forever.

    return bridge_interrupts(sxb, prog_addr);
}

} // namespace w65c02

#endif /* w65c02sxbenv_hpp */
