// fo16.hpp
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


#ifndef fo16_hpp
#define fo16_hpp

#include "pieces.hpp"

#include <array>
#include <iostream>

namespace fauces
{

struct Fo16_error_bad {};
struct Fo16_error_overflow {};

enum class Whence
{
    start,
    cur
};

class Content_access
{
public:
    Content_access(const std::vector<unsigned char>& content): content {content}
    {}
    unsigned short seek(unsigned short pos, Whence whence)
    {
        if (whence == Whence::cur)
        {
            pos = pointer + pos;
            if (pos < pointer)
                throw std::out_of_range("Address not in content");
        }
        if (pos > content.size())
            throw std::out_of_range("Address not in content");
        pointer = pos;
        return pointer;
    }
    unsigned char load_byte()
    {
        return content.at(pointer++);
    }
    unsigned short load_short()
    {
        unsigned short n = (content.at(pointer) << 8) | content.at(pointer + 1);
        pointer += 2;
        return n;
    }
    std::string load_string()
    {
        std::string str;
        unsigned char c;
        while ((c = content.at(pointer++)))
        {
            str += c;
        }
        return str;
    }
private:
    unsigned short pointer {0};
    const std::vector<unsigned char>& content;
};

struct Reference_record
{
    unsigned char type;
    unsigned short section_id;
    unsigned short location;
    Reference_record(Content_access& a, unsigned char type) :
    type {type},
    section_id {a.load_short()},
    location {a.load_short()}
    {}
};

struct Symbol_record
{
    static constexpr unsigned short strings = 4;
    std::string name;
    unsigned short section_id;
    unsigned short location;
    unsigned short object_size;
    std::vector<Reference_record> references;
    Symbol_record(Content_access& a)
    {
        unsigned short name_pos = a.load_short();
        Content_access s {a};
        s.seek(strings + name_pos, Whence::start);
        name = s.load_string();
        section_id = a.load_short();
        location = a.load_short();
        object_size = a.load_short();
        unsigned char number = a.load_byte();
        unsigned char type = a.load_byte();
        while (number != 0 && type != 0)
        {
            Reference_record r {a, type};
            for (unsigned char i = 0; i < number; ++i)
            {
                references.push_back(r);
                r = {a, type};
            }
            number = a.load_byte();
            type = a.load_byte();
        }
    }
};

class Fo16_unit_loader : public Translated_unit_loader
{
public:
    Fo16_unit_loader(const std::string& path) : path {path} {}
    static bool is_signature(const std::array<unsigned char, 16>& try_signature)
    {
        static const std::array<unsigned char, 16> signature =
        {
            0xec, 0x4b, 0x79, 0xc8, 0x57, 0xee, 0x4b, 0xad,
            0x96, 0xcf, 0x93, 0x85, 0x48, 0xa2, 0xe0, 0x4b
        };
        return try_signature == signature;
    }
private:
    const std::string path;
    unsigned short code_id {0xffff};
    unsigned short data_id {0xffff};
    std::vector<Symbol_record> symrec;

    std::unique_ptr<Translated_unit> load() override;
    void init(std::ifstream &ifs);
    bool load_section
                (std::istream& is, Translated_unit* unit, unsigned short id);
    void load_symbols
            (Translated_unit* unit, const std::vector<unsigned char>& content);
};

}

#endif /* fo16_hpp */
