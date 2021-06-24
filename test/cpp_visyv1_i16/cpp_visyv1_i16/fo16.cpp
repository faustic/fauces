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
#include "../../visy1010/visy1010/using_algorithm.hpp"

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

static unsigned short load_short(istream& is)
{
    unsigned char bytes[2];
    read(is, bytes, 2);
    return (bytes[0] << 8) | bytes[1];
}

static void save_short(ostream& os, unsigned short n)
{
    unsigned char bytes[2]
            {static_cast<unsigned char>(n >> 8), static_cast<unsigned char>(n)};
    write(os, bytes, 2);
}

void Fo16_unit_loader::load_symbols
                (Translated_unit* unit, const vector<unsigned char>& content)
{
    Content_access sym(content);
    unsigned short num_symbols = sym.load_short();
    unsigned short size = sym.load_short();
    sym.seek(size, Whence::cur);
    for (unsigned short i = 0; i < num_symbols; ++i)
        symrec.emplace_back(sym);
}

bool Fo16_unit_loader::load_section
                        (istream& is, Translated_unit* unit, unsigned short id)
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
            code_id = id;
            swap(unit->code, content);
            break;
        case Sec_type::data:
            if (unit->data.size())
                throw Fo16_error_bad();
            data_id = id;
            swap(unit->data, content);
            break;
        case Sec_type::symbols:
            load_symbols(unit, content);
            break;
        case Sec_type::eof:
            return false;
        default:
            throw Fo16_error_bad();
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
        ;
    for (auto i = symrec.begin(); i != symrec.end(); ++i)
    {
        Sym_type stype;
        if (i->section_id == code_id)
            stype = Sym_type::code;
        else if (i->section_id == data_id)
            stype = Sym_type::data;
        else
            throw Fo16_error_bad();
        Symbol symbol {i->location, i->object_size, stype};
        for (auto j = i->references.begin(); j != i->references.end(); ++j)
        {
            Reference ref {Ref_type {j->type}, j->location};
            if (j->section_id == code_id)
                symbol.references_in_code.push_back(ref);
            else if (j->section_id == data_id)
                symbol.references_in_data.push_back(ref);
            else
                throw Fo16_error_bad();
        }
        swap(unit->symbols[i->name], symbol);
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

void fauces::Fo16_program_saver::init(std::ofstream &ofs)
{
    ofs.exceptions(ios::failbit | ios::badbit);
    try
    {
        ofs.open(path, ios::binary | ios::trunc);
    }
    catch (...)
    {
        throw File_error_cantopen();
    }
    try
    {
        const auto& data = Fo16_unit_loader::signature.data();
        const auto size = Fo16_unit_loader::signature.size();
        write(ofs, data, size);
        save_short(ofs, static_cast<unsigned short>(Cpu_type::visy));
        save_short(ofs, pref_code_id);
        save_short(ofs, pref_start);
    }
    catch (...)
    {
        throw File_error_write();
    }
}

void fauces::Fo16_program_saver::save(Linked_program &prog)
{
    if (prog.ext_symbols.size())
        throw Ref_unresolved();
    if (!prog.code.size())
        throw Prog_nocode();
    if (prog.code.size() > 65536)
        throw Prog_toobig();
    ofstream ofs;
    init(ofs);
    static_assert(pref_code_id == 0);
    unsigned short section_id = pref_code_id;
    save_section(ofs, section_id, Sec_type::code, prog.code);
    if (prog.data.size())
        save_section(ofs, ++section_id, Sec_type::data, prog.data);
    save_eof(ofs, ++section_id);
    try
    {
        ofs.close();
    }
    catch (...)
    {
        throw File_error_write();
    }
}

void fauces::Fo16_program_saver::save_section(std::ofstream &ofs,
    unsigned char id, Sec_type type, std::vector<unsigned char>& bytes)
{
    save_short(ofs, id);
    save_short(ofs, static_cast<unsigned short>(type));
    if (bytes.size() % 2)
        bytes.push_back(0);
    save_short(ofs, bytes.size() / 2);
    save_short(ofs, pref_start);
    write(ofs, bytes.data(), bytes.size());
}

void fauces::Fo16_program_saver::save_eof(std::ofstream &ofs, unsigned char id)
{
    save_short(ofs, id);
    save_short(ofs, static_cast<unsigned short>(Sec_type::eof));
    unsigned short size = 0;
    save_short(ofs, size);
    save_short(ofs, pref_start);
}
