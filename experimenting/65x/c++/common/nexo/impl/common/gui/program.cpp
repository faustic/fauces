// program.cpp
// Hidden implementation of program-related classes
// Created by Alejandro Castro Garcia on 22 October 2020
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

#include <nexo/program.hpp>

namespace nexo
{

using std::vector;
using std::shared_ptr;

int Program::priv_result;
int Program::priv_argc;
char** Program::priv_argv;
vector<shared_ptr<Lifecycle>> Program::lifecycle;

class Run_exit {};

void Program::add_lifecycle(std::shared_ptr<Lifecycle>& lf)
{
    lifecycle.push_back(lf);
}

void Program::cleanup()
{
    // Lifecycle objects might require proper cleanup
    lifecycle.resize(0);
}

bool Program::please_quit()
{
    for (auto& lc: lifecycle)
    {
        if (!lc->please_quit())
            return false;
    }
    return true;
}

void Program::ready()
{
    for (auto& lc: lifecycle)
        lc->ready();
}

void Program::noparam()
{
    for (auto& lc: lifecycle)
        lc->noparam();
}

void Program::run(int argc, char** argv)
{
    priv_argc = argc;
    priv_argv = argv;
    try
    {
        event_loop();
    }
    catch(Run_exit)
    {
        cleanup();
        // Normal termination just unwinds the stack and returns to the caller
    }
}

void Program::exit(int ret_value)
{
    priv_result = ret_value;
    for (auto& lc: lifecycle)
        lc->quitting();
    throw Run_exit();
}


} // namespace nexo

