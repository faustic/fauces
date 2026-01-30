// User interface: implementation for the command line
//
// Created by Alejandro Castro García on 26 May 2023
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


#include "ui.hpp"
#include "machine.hpp"

#include <iostream>

using std::cout;
using std::cin;

void w65c02::User_interface::start_events()
{
    if (listening)
    {
        cout << "Accepted commands: \n";
        cout << "* i: IRQ low\n";
        cout << "* h: IRQ high\n";
        cout << "* n: NMI\n";
        cout << "* r: reset CPU\n";
        cout << "* c: continue\n";
        cout << "* q: shutdown machine\n";
        cout << "\n";
        listening = true;
    }
}

void w65c02::User_interface::next_event()
{
    if (listening)
    {
        char c;
        cin >> c;
        switch (c)
        {
            case 'r':
                m.cpu().reset_low();
                m.cpu().reset_high();
                break;
            case 'i':
                m.cpu().irq_low();
                break;
            case 'h':
                m.cpu().irq_high();
                break;
            case 'n':
                m.cpu().nmi_low();
                m.cpu().nmi_high();
                break;
            case 'c':
                // continue (do nothing here)
                break;
            case 'q':
                m.power(false);
                break;
            default:
                cout << "Unknown command: " << c << "\n";
        }
    }
}

void w65c02::User_interface::end_events()
{
}

void w65c02::User_interface::msg(const string& s)
{
    if (listening)
        cout << s << "\n";
}
