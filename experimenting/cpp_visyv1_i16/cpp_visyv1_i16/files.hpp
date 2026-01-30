// files.hpp
// File transformation.
// Intended compatibility: c++17
//
// Created by Alejandro Castro García on 15 April 2021
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


#ifndef files_hpp
#define files_hpp

#include <string>
#include <unordered_map>
#include <iostream>

#include "pieces.hpp"

namespace fauces
{

struct Program_input
{
    std::unordered_map<std::string, std::string> options;
    std::string value;
};

struct Program_output
{
    std::string value;
};

struct File_error_cantopen {};
struct File_error_read {};
struct File_error_write {};
struct File_error_unknown {};

void add_to_supply(Supply& supply, const Program_input& input);
void save_program(Linked_program& prog, const Program_output& output);

} // namespace fauces
#endif /* files_hpp */
