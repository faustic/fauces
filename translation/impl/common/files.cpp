// files.cpp
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


#include "files.hpp"

#include "fo16.hpp"
#include "translator.hpp"

#include "../../../experimenting/visy1010/visy1010/using_iostream.hpp"
#include "../../../experimenting/visy1010/visy1010/using_containers.hpp"
#include "../../../experimenting/visy1010/visy1010/using_string.hpp"
#include "../../../experimenting/visy1010/visy1010/using_memory.hpp"


namespace fauces
{

enum class File_type {fo16, cpp};

static File_type identify_source_file(string filename)
{
    return File_type::cpp;
}

static File_type identify_file_type(string filename)
{
    ifstream ifs;
    ifs.exceptions(ios::failbit | ios::badbit | ios::eofbit);
    try
    {
        ifs.open(filename, ios::binary);
    }
    catch (...)
    {
        throw File_error_cantopen();
    }
    try
    {
        array<unsigned char, 16> signature;
        read(ifs, signature.data(), signature.size());
        if (Fo16_unit_loader::is_signature(signature))
            return File_type::fo16;
    }
    catch (...)
    {
        if (!ifs.eof())
            throw File_error_read();
    }
    return identify_source_file(filename);
}

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
            loader = make_unique<Translator>(input.value);
            break;
        default:
            throw File_error_unknown();
    }
    supply.add_unit(loader->load());
}

void save_program(Linked_program& prog, const Program_output& output)
{
    Fo16_program_saver fo16_saver {output.value};
    Linked_program_saver& saver = fo16_saver;
    saver.save(prog);
}

} // namespace fauces
