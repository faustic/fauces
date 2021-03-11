// exec.cpp
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


#include "exec.hpp"

#include "using_cstdint.hpp"
#include "using_iostream.hpp"
#include "using_algorithm.hpp"
#include "using_containers.hpp"

namespace vs
{
static const unsigned char exe_uuid[16] =
{
    0xec, 0x4b, 0x79, 0xc8, 0x57, 0xee, 0x4b, 0xad,
    0x96, 0xcf, 0x93, 0x85, 0x48, 0xa2, 0xe0, 0x4b
};

enum class Exe_addr
{
    undefined = 0xffff
};

enum Exe_cpu
{
    visy_v1
};

struct Exe_header
{
    unsigned char uuid[16];
    unsigned cpu;
    unsigned start_section;
    unsigned start_addr;
};

enum Exe_section_type
{
    code_section = 0x434f,
    data_section = 0x4441,
    eof_section = 0x454f
};

struct Code_section
{
    Section_header header;
    unsigned code_size;
};

unsigned read_be2(istream& is)
{
    unsigned char tmp;
    is >> tmp;
    unsigned value = static_cast<unsigned>(tmp) << 8;
    is >> tmp;
    value |= tmp;
    return value;
}

Section_header load_section_header(istream& is)
{
    Section_header header;
    header.id = read_be2(is);
    header.type = read_be2(is);
    header.size = read_be2(is);
    header.pref_start = read_be2(is);
    header.relocs = read_be2(is);
    return header;
}

Program::Program(char* filename)
{
    cout << "filename: " << filename << "\n";
    ifstream ifs;
    ifs.exceptions(ios::failbit | ios::badbit | ios::eofbit);
    ifs.open(filename, ios::binary);
    unsigned start_section = load_exe_header(ifs);
    unsigned expected_id = 0;
    bool has_code_section = false;
    for (Section_header h = load_section_header(ifs); h.type != eof_section;
         h = load_section_header(ifs), ++expected_id)
    {
        if (h.id != expected_id)
            throw Program_loading_error("Unexpected section identifier");
        if (h.relocs != 0 && h.pref_start != 0)
            throw Program_loading_error("Relocations not supported");
        if (h.type == code_section)
        {
            has_code_section = true;
            if (h.id != start_section)
                throw Program_loading_error("Must start at code section");
        }
        process_section(ifs, h);
    }
    if (!has_code_section)
        throw Program_loading_error("No code section");
}

unsigned Program::load_exe_header(istream& is)
{
    Exe_header header;
    is.read(reinterpret_cast<char*>(header.uuid), 16);
    if (!equal(header.uuid, header.uuid + 16, exe_uuid))
        throw Program_loading_error("Unknown format identifier");
    header.cpu = read_be2(is);
    if (header.cpu != visy_v1)
        throw Program_loading_error("Unsupported CPU");
    header.start_section = read_be2(is);
    priv_start = header.start_addr = read_be2(is);
    return header.start_section;
}

void Program::process_section(istream &is, Section_header& h)
{
    switch (h.type)
    {
        case code_section:
            load_section(is, priv_code, h.size);
            break;
        case data_section:
            load_section(is, priv_data, h.size);
            break;
        default:
            if (h.size)
                is.seekg(h.size, ios::cur);
    }
}

void Program::load_section(istream &is, vector<unsigned char>& mem,
                                                                unsigned size)
{
    if (size)
    {
        if (mem.size())
            throw Program_loading_error("Section of same type already loaded");
        mem.resize(size);
        is.read(reinterpret_cast<char*>(mem.data()), size);
    }
}

} // namespace vs
