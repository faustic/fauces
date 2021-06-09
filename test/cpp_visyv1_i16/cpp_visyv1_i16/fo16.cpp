// fo16.cpp
// Fo16 format support
// Intended compatibility: c++17
//
// Created by Alejandro Castro García on 14 April 2021
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


#include "fo16.hpp"
#include "files.hpp"

#include "../../visy1010/visy1010/using_iostream.hpp"
#include "../../visy1010/visy1010/using_containers.hpp"
#include "../../visy1010/visy1010/using_string.hpp"
#include "../../visy1010/visy1010/using_memory.hpp"

namespace fauces
{

enum class Cpu_type
{
    visy = 0
};

enum Sec_id
{
    sec_max_id = 65534
};

enum class Sec_type
{
    code = 0x434f,
    data = 0x4441,
    symbols = 0x5359,
    eof = 0x454f
};

static unsigned short load_short(istream& is)
{
    unsigned char bytes[2];
    read(is, bytes, 2);
    return (bytes[0] << 8) | bytes[1];
}

static bool load_section(istream& is, Translated_unit* unit, unsigned short id)
{
    if (id > sec_max_id || id != load_short(is))
        throw Fo16_error_bad();
    Sec_type type = static_cast<Sec_type>(load_short(is));
    unsigned short size = load_short(is);
    load_short(is);
    using mtype = vector<unsigned char>::size_type;
    mtype msize = size * static_cast<mtype>(2);
    if (msize < size)
        throw Fo16_error_overflow();
    vector<unsigned char> content(msize);
    read(is, content.data(), msize);
    switch (type)
    {
        case Sec_type::code:
            if (unit->code.size())
                throw Fo16_error_bad();
            unit->code = content;
            break;
        case Sec_type::data:
            if (unit->data.size())
                throw Fo16_error_bad();
            unit->data = content;
            break;
        case Sec_type::eof:
            return false;
    }
    return true;
}

unique_ptr<Translated_unit> Fo16_unit_loader::load()
{
    ifstream ifs;
    init(ifs);
    
    cout << "load\n";
    
    unique_ptr<Translated_unit> unit = make_unique<Translated_unit>();
    
    for (unsigned short i = 0; load_section(ifs, unit.get(), i); ++i)
    {
        
    }
    
    return unit;
}

void Fo16_unit_loader::init(ifstream& ifs)
{
    ifs.exceptions(ios::failbit | ios::badbit | ios::eofbit);
    try
    {
        ifs.open(path, ios::binary);
    }
    catch (...)
    {
        throw File_error_cantopen();
    }
    try
    {
        array<unsigned char, 16> signature;
        read(ifs, signature.data(),signature.size());
        if (!Fo16_unit_loader::is_signature(signature))
            throw File_error_unknown();
        Cpu_type cpu = static_cast<Cpu_type>(load_short(ifs));
        unsigned short start_section = load_short(ifs);
        unsigned short start_offset = load_short(ifs);
        if (cpu != Cpu_type::visy || start_section != 0xffff ||
                                                        start_offset != 0xffff)
            throw Fo16_error_bad();
    }
    catch (...)
    {
        throw File_error_read();
    }
}

} // namespace fauces
