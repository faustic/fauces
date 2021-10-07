// pieces.hpp
// Classes representing the pieces used to build a program
// Intended compatibility: c++17
//
// Created by Alejandro Castro García on 13 April 2021
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


#ifndef pieces_hpp
#define pieces_hpp

#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <memory>
#include <cstddef>

namespace fauces
{

using Location =  std::uint_least16_t;
using Size =  std::uint_least16_t;
using std::string;
using std::size_t;
using std::vector;
using std::unique_ptr;
using std::make_unique;

enum class Ref_type
{
    two_bytes,
    four_halfbytes
};

struct Reference
{
    Ref_type type;
    Location pos;
    Reference(Ref_type type, Location pos): type {type}, pos {pos}
    {}
};

enum class Sym_type
{
    code,
    data
};

enum class Token_type
{
    incomplete,
    unknown,
    white,
    bom,
    eof,
    pp_number,
    identifier,
    pp_op_or_punc
};

struct Source_location
{
    string path;
    size_t lineno;
    size_t col;
    
    Source_location(const string& path, size_t lineno = 0, size_t col = 0):
    path {path},
    lineno {lineno},
    col {col}
    {}
};

struct Source_context
{
    Source_location src;
    size_t line_start = 0;
    bool literal = false;
    
    Source_context(const string& path) :
    src {path}
    {}
};

struct Token
{
    string text;
    Source_location src;
    Token_type type;
    
    Token(string source, size_t line, size_t col):
    src {source, line, col},
    type {Token_type::incomplete}
    {}
    
    Token(Source_location& src):
    src {src},
    type {Token_type::incomplete}
    {}
};

struct Ref_type_bad {};
struct Sym_type_bad {};
struct Ref_unresolved {string symbol_name;};
struct Prog_nocode {};
struct Prog_toobig {};
struct Syntax_error {string msg;};
struct Limit_error {string msg;};

struct Symbol
{
    Location pos;
    Size size;
    Sym_type type;
    std::vector<Reference> references_in_code;
    std::vector<Reference> references_in_data;
    std::unordered_map<string, std::vector<Reference>> references_to_others;
    bool is_external()
    {
        return pos == 0 && size == 0;
    }
};

struct Linked_symbol
{
    Location pos;
    Size size;
    Sym_type type;
    std::unordered_map<string, std::vector<Reference>> refs;
};

struct Translated_unit_error {};

struct Linked_program
{
    
    void load_symbol(const string& name, const Symbol& symbol,
                                    const std::vector<unsigned char>& origin);
    const std::unordered_map<string, bool>& pending_symbols()
    {
        return ext_symbols;
    }
    
    const std::vector<unsigned char>& code_section()
    {
        return code;
    }
    
    const std::vector<unsigned char>& data_section()
    {
        return data;
    }
    
    void verify()
    {
        if (ext_symbols.size())
            throw Ref_unresolved();
        if (!code.size())
            throw Prog_nocode();
        if (code.size() % 2)
            code.push_back(0);
        if (data.size() % 2)
            code.push_back(0);
        if (code.size() > 65536 || data.size() > 65536)
            throw Prog_toobig();
    }
    
private:
    std::vector<unsigned char> code;
    std::vector<unsigned char> data;
    std::unordered_map<string, Linked_symbol> int_symbols;
    std::unordered_map<string, bool> ext_symbols;
    
    std::vector<unsigned char>* section_bytes(Sym_type type);

    void relocate(const Linked_symbol& caller, const Linked_symbol& called,
                                                        const Reference& ref);
    Linked_symbol init_linked_symbol
        (const Symbol& symbol, const std::vector<unsigned char>& origin);
    void relocate_new_references(const Symbol& sym, Linked_symbol& lsym);
    void relocate_old_references
        (const string& name, const Linked_symbol& lsym);
};

class Linked_program_saver
{
public:
    virtual void save(Linked_program& prog) = 0;
    virtual ~Linked_program_saver() = default;
};

struct Translated_unit
{
    std::vector<unsigned char> code;
    std::vector<unsigned char> data;
    std::unordered_map<string, Symbol> symbols;
};

class Translated_unit_loader
{
public:
    virtual std::unique_ptr<Translated_unit> load() = 0;
    virtual ~Translated_unit_loader() = default;
};

using Instantiation_unit = Translated_unit;

class Supply
{
public:
    void add_unit(std::unique_ptr<Translated_unit> unit)
    {
        units.push_back(move(unit));
    }
    
    void clear()
    {
        units.clear();
    }
    
    Linked_program link();

private:
    std::vector<std::unique_ptr<Translated_unit>> units;
    
    void add_start(Linked_program& prog)
    {
        add_symbol(prog, "_start");
    }
    
    void add_symbol(Linked_program& prog, const string& symbol_name);
};

template<typename T>
bool within(T thing, T begin, T size)
{
    return thing >= begin && thing < begin + size;
}

class Relocate
{
public:
    virtual
    void change_loc
        (std::vector<unsigned char>& dst, Location pos, Location new_ref) = 0;
    virtual ~Relocate() = default;
};

class Relocate_two_bytes: public Relocate
{
    void change_loc
        (std::vector<unsigned char>& dst, Location pos, Location new_ref)
        override
    {
        dst.at(pos) = (new_ref >> 8) & 0xff;
        dst.at(pos + 1) = new_ref & 0xff;
    }
};

class Relocate_four_halfbytes: public Relocate
{
    void change_loc
        (std::vector<unsigned char>& dst, Location pos, Location new_ref)
        override
    {
        constexpr unsigned char mask = 0b0001'1110;
        auto& b3 = dst.at(pos);
        auto& b2 = dst.at(pos + 1);
        auto& b1 = dst.at(pos + 2);
        auto& b0 = dst.at(pos + 3);
        b0 = (b0 & ~mask) | ((new_ref << 1) & mask);
        b1 = (b1 & ~mask) | ((new_ref >> 3) & mask);
        b2 = (b2 & ~mask) | ((new_ref >> 7) & mask);
        b3 = (b3 & ~mask) | ((new_ref >> 11) & mask);
    }
};

} // namespace fauces

#endif /* pieces_hpp */
