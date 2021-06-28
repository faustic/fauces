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

void fauces::Linked_program::load_symbol
                    (const Symbol& symbol, const vector<unsigned char>& origin)
{
    auto end = symbol.pos + symbol.size;
    if (symbol.references_in_code.size() || symbol.references_in_data.size() ||
                                            symbol.references_to_others.size())
        throw std::runtime_error("References not supported yet");
    Symbol dst {symbol};
    vector<unsigned char>* bytes = nullptr;
    switch (symbol.type)
    {
        case Sym_type::code:
            dst.pos = code.size();
            bytes = &code;
            break;
        case Sym_type::data:
            dst.pos = data.size();
            bytes = &data;
            break;
        default:
            throw Sym_type_bad();
    }
    for (auto i = symbol.pos; i != end; ++i)
    {
        bytes->push_back(origin.at(i));
    }
    // Pending:
    // - Perform relocations from already loaded symbols (int_symbols).
    // - Add current symbol to list of internal symbols.
    // - Perform relocations needed by previously loaded symbols.
    // - Add unresolved references to ext_symbols.
}


fauces::Linked_program fauces::Supply::link()
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

void fauces::Supply::add_symbol(Linked_program& prog, string symbol_name)
{
    for (auto i = units.begin(); i != units.end(); ++i)
    {
        auto& symbols = (*i)->symbols;
        auto j = symbols.find(symbol_name);
        if (j != symbols.end() && !j->second.is_external())
        {
            auto& sym = j->second;
            auto& bytes = sym.type == Sym_type::code ? (*i)->code : (*i)->data;
            prog.load_symbol(sym, bytes);
            prog.ext_symbols.erase(symbol_name);
            return;
        }
    }
    throw Ref_unresolved {symbol_name};
}
