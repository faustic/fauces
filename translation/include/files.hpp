// files.hpp
// File transformation.
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
#include "translator.hpp"
#include "fo16.hpp"
#include "arch.hpp"

namespace fauces
{

inline void read(std::istream& is, unsigned char* data, std::streamsize size)
{
    is.read(reinterpret_cast<char*>(data),size);
}

inline void write
            (std::ostream& os, const unsigned char* data, std::streamsize size)
{
    os.write(reinterpret_cast<const char*>(data),size);
}

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

enum class File_type {fo16, cpp};

File_type identify_file_type(string filename);

template<typename Arch>
void add_to_supply(Supply& supply, const Program_input& input)
{
    File_type type = identify_file_type(input.value);
    unique_ptr<Translated_unit_loader> loader;
    switch (type)
    {
        case File_type::fo16:
            loader = make_unique<Fo16_unit_loader>(input.value);
            break;
        case File_type::cpp:
            loader = make_unique<Translator<Arch>>(input.value);
            break;
        default:
            throw File_error_unknown();
    }
    supply.add_unit(loader->load());
}

template<typename Arch>
void save_program(Linked_program& prog, const Program_output& output)
{
    Fo16_program_saver fo16_saver {output.value};
    Linked_program_saver& saver = fo16_saver;
    saver.save(prog);
}

} // namespace fauces
#endif /* files_hpp */
