// phase9.cpp
// Phase 9 of translation
//
// Created by Alejandro Castro García on 9 February 2021
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


#include "phase9.hpp"

#include "pieces.hpp"

void
fauces::Linked_program::
load_symbol
(const string& name, const Symbol& symbol, const vector<unsigned char>& origin)
{
    Linked_symbol lsym = init_linked_symbol(symbol, origin);
    relocate_new_references(symbol, lsym);
    int_symbols.emplace(name, lsym);
    relocate_old_references(name, lsym);
}

auto fauces::Linked_program::
init_linked_symbol(const Symbol &symbol, const vector<unsigned char> &origin)
-> Linked_symbol
{
    Linked_symbol linked_symbol {symbol.pos, symbol.size, symbol.type};
    auto bytes = section_bytes(symbol.type);
    linked_symbol.pos = bytes->size();
    auto sym_end = symbol.pos + symbol.size;
    for (auto i = symbol.pos; i != sym_end; ++i)
        bytes->push_back(origin.at(i));
    return linked_symbol;
}

void
fauces::Linked_program::
relocate_new_references(const Symbol& symbol, Linked_symbol& linked_symbol)
{
    auto& refs = symbol.references_to_others;
    for (auto i = refs.begin(); i != refs.end(); ++i)
    {
        auto other_name = i->first;
        auto ref_symbol = int_symbols.find(other_name);
        if (ref_symbol != int_symbols.end())
        {
            auto& ref_list = i->second;
            for (auto j = ref_list.begin(); j != ref_list.end(); ++j)
                relocate(linked_symbol, ref_symbol->second, *j);
        }
        else
        {
            ext_symbols.emplace(other_name, true);
            linked_symbol.refs[other_name] = i->second;
        }
    }
}

void
fauces::Linked_program::
relocate_old_references(const string& name, const Linked_symbol& lsym)
{
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
                    relocate(i->second, lsym, *j);
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

auto
fauces::Linked_program::
section_bytes(Sym_type type) -> vector<unsigned char>*
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

