// pieces.cpp
// Classes representing the pieces used to build a program
// Intended compatibility: c++17
//
// Created by Alejandro Castro García on 13 April 2021
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


#include "pieces.hpp"

#include "../../visy1010/visy1010/using_containers.hpp"
#include "../../visy1010/visy1010/using_string.hpp"
#include "../../visy1010/visy1010/using_memory.hpp"

void
fauces::Linked_program::
load_symbol
(const string& name, const Symbol& symbol, const vector<unsigned char>& origin)
{
    Linked_symbol dst {symbol.pos, symbol.size, symbol.type};
    auto bytes = section_bytes(symbol.type);
    dst.pos = bytes->size();
    auto sym_end = symbol.pos + symbol.size;
    for (auto i = symbol.pos; i != sym_end; ++i)
        bytes->push_back(origin.at(i));
    auto& refs = symbol.references_to_others;
    for (auto i = refs.begin(); i != refs.end(); ++i)
    {
        auto other_name = i->first;
        auto ref_symbol = int_symbols.find(other_name);
        if (ref_symbol != int_symbols.end())
        {
            auto& ref_list = i->second;
            for (auto j = ref_list.begin(); j != ref_list.end(); ++j)
                relocate(dst, ref_symbol->second, *j);
        }
        else
        {
            ext_symbols.emplace(other_name, true);
            dst.refs[other_name] = i->second;
        }
    }
    int_symbols.emplace(name, dst);
    if (ext_symbols.find(name) != ext_symbols.end())
    {
        for (auto i = int_symbols.begin(); i != int_symbols.end(); ++i)
        {
            auto& prev_refs = i->second.refs;
            auto prev_ref = prev_refs.find(name);
            if (prev_ref != prev_refs.end())
            {
                auto& ref_list = prev_ref->second;
                for (auto j = ref_list.begin(); j != ref_list.end(); ++j)
                    relocate(i->second, dst, *j);
                prev_refs.erase(name);
            }
        }
        ext_symbols.erase(name);
    }
}

void
fauces::Linked_program::
relocate
(const Linked_symbol& caller,const Linked_symbol& called, const Reference& ref)
{
    auto bytes = section_bytes(caller.type);
    unique_ptr<Relocate> reloc;
    switch (ref.type)
    {
        case Ref_type::two_bytes:
            reloc = make_unique<Relocate_two_bytes>();
            break;
        case Ref_type::four_halfbytes:
            reloc = make_unique<Relocate_four_halfbytes>();
            break;
        default:
            throw Ref_type_bad();
    }
    reloc->change_loc(*bytes, caller.pos + ref.pos, called.pos);
}

vector<unsigned char>*
fauces::Linked_program::
section_bytes(Sym_type type)
{
    switch (type)
    {
        case Sym_type::code:
            return &code;
            break;
        case Sym_type::data:
            return &data;
            break;
        default:
            throw Sym_type_bad();
    }
}


fauces::Linked_program
fauces::Supply::link()
{
    Linked_program prog;
    add_start(prog);
    while (prog.ext_symbols.size())
    {
        auto ext_sym = prog.ext_symbols;
        for (auto i = ext_sym.begin(); i != ext_sym.end(); ++i)
            add_symbol(prog, i->first);
    }
    clear();
    return prog;
}

void
fauces::Supply::
add_symbol(Linked_program& prog, const string &symbol_name)
{
    for (auto i = units.begin(); i != units.end(); ++i)
    {
        auto& symbols = (*i)->symbols;
        auto j = symbols.find(symbol_name);
        if (j != symbols.end() && !j->second.is_external())
        {
            auto& sym = j->second;
            auto& bytes = sym.type == Sym_type::code ? (*i)->code : (*i)->data;
            prog.load_symbol(symbol_name, sym, bytes);
            return;
        }
    }
    throw Ref_unresolved {symbol_name};
}
