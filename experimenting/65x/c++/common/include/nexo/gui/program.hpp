// program.hpp
// Program-related classes
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



#ifndef nexo_program_hpp
#define nexo_program_hpp

#include <vector>
#include <memory>

namespace nexo
{

class Lifecycle
{
public:
    virtual bool please_quit()
    {
        return true;
    }
    virtual void quitting()
    {
    }
    virtual void ready()
    {
    }
    virtual void noparam()
    {
    }
};

class Program
{
public:
    static void add_lifecycle(std::shared_ptr<Lifecycle>& lf);
    static int argc()
    {
        return priv_argc;
    }
    static char** argv()
    {
        return priv_argv;
    }
    static void cleanup();
    static bool please_quit();
    static int result()
    {
        return priv_result;
    }
    static void run(int argc, char** argv);
    static void ready();
    static void noparam();
    static void exit(int ret_value = 0);
    
private:
    static int priv_result;
    static int priv_argc;
    static char** priv_argv;
    static std::vector<std::shared_ptr<Lifecycle>> lifecycle;

    static void event_loop();
}; // class Program

} // namespace nexo

#endif /* nexo_program_hpp */
