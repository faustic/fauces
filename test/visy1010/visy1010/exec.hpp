// exec.hpp
// Loading of executable file
// Intended compatibility: c++17
//
// Created by Alejandro Castro García on 22 February 2021
/*
Licensed under the MIT License.
 
Copyright (c) 2021 Faustic Inferno SL
 
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


#ifndef fauvisy_exec_hpp
#define fauvisy_exec_hpp

#include <vector>
#include <iostream>
#include <string>

namespace vs
{

struct Program_loading_error
{
    std::string msg;
    Program_loading_error(const std::string& msg): msg {msg}
    {}
};

struct Section_header
{
    unsigned id;
    unsigned type;
    unsigned size;
    unsigned pref_start;
    unsigned relocs;
};

class Program
{
    std::vector<unsigned char> code;
    std::vector<unsigned char> data;
    unsigned start;
    
    unsigned load_exe_header(std::istream& is);
    void process_section(std::istream& is, Section_header& h);
    void load_section(std::istream &is, std::vector<unsigned char>& mem,
                                                                unsigned size);

public:
    Program(char* filename);
};

} // namespace vs

#endif /* fauvisy_exec_hpp */
