// pieces.hpp
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


#ifndef pieces_hpp
#define pieces_hpp

#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <memory>

namespace fauces
{

using Location =  std::uint_least16_t;
using Size =  std::uint_least16_t;

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

struct Ref_type_bad {};
struct Sym_type_bad {};
struct Ref_unresolved {std::string symbol_name;};
struct Prog_nocode {};
struct Prog_toobig {};

struct Symbol
{
    Location pos;
    Size size;
    Sym_type type;
    std::vector<Reference> references_in_code;
    std::vector<Reference> references_in_data;
    std::unordered_map<std::string, std::vector<Reference>>
                                                        references_to_others;
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
    std::unordered_map<std::string, std::vector<Reference>> refs;
};

struct Translated_unit_error {};

struct Linked_program
{
    std::vector<unsigned char> code;
    std::vector<unsigned char> data;
    std::unordered_map<std::string, Linked_symbol> int_symbols;
    std::unordered_map<std::string, bool> ext_symbols;
    
    std::vector<unsigned char>* section_bytes(Sym_type type);
    
    void load_symbol(const std::string& name, const Symbol& symbol,
                                    const std::vector<unsigned char>& origin);
    
    void relocate(const Linked_symbol& caller, const Linked_symbol& called,
                                                        const Reference& ref);
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
    std::unordered_map<std::string, Symbol> symbols;
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
    
    void add_symbol(Linked_program& prog, const std::string& symbol_name);
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
