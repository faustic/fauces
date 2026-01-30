// Program entry point
//
// Created by Alejandro Castro García on 28 April 2023
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

#include "machine.hpp"

#include <iostream>
#include <cstdlib>
#include <string>

using std::string;

static bool is_switch(const string name, const string& arg, bool& fail)
{
    if (arg == "--" + name)
    {
        fail = true;
        return true;
    }
    else if (arg == "--no-" + name)
    {
        fail = false;
        return true;
    }
    else
        return false;
}

int main(int argc, char** argv)
{
    using w65c02::Test_machine;
    bool fail = true;
    bool rockwell = false;
    bool pausable = false;
    bool ui = false;
    for (int i = 1; i < argc; ++i)
    {
        string arg {argv[i]};
        if (is_switch("fail", arg, fail))
            continue;
        if (is_switch("rockwell", arg, rockwell))
            continue;
        if (is_switch("pausable", arg, pausable))
            continue;
        if (is_switch("ui", arg, ui))
            continue;
        try
        {
            Test_machine machine(arg, fail, rockwell, pausable, ui);
            std::cout << argv[i] << "\n";
            machine.turn_on();
        }
        catch (std::runtime_error& e)
        {
            std::cerr << "Failure: " << argv[i] << ": " << e.what() << "\n";
            return EXIT_FAILURE;
        }
        catch (...)
        {
            std::cerr << "Failure: " << argv[i] << ": Unknown error\n";
            return EXIT_FAILURE;
        }
    }
    return 0;
}
