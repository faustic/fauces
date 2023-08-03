// phase1.cpp
// Phase 1 of translation
// Intended compatibility: c++17
//
// Created by Alejandro Castro García on 9 February 2021
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


#include "phase1.hpp"

#include <type_traits>

static inline char plainchar(unsigned char c)
{
    if (std::is_unsigned<char>::value)
        return c;
    else
        return *reinterpret_cast<char*>(&c);
}

static inline bool is_continuation(int c)
{
    return ((c & 0xc0) == 0x80);
}

namespace fauces
{
static char32_t add_continuation(istream& is, char32_t& c);
static char32_t from_utf8(istream& is);
}

static inline char32_t
    fauces::add_continuation(istream& is, char32_t& c)
{
    int u8 = is.get();
    if (!is_continuation(u8))
        throw Invalid_character();
    c = (c << 6) | (static_cast<char32_t>(u8) & 0x3f);
    return c;
}

static char32_t fauces::from_utf8(istream& is)
{
    // Exceptions must have been set for EOF
    int u8 = is.get();
    // No need to check for EOF because exceptions are set
    if (!(u8 & 0x80))
        return static_cast<char32_t>(u8);
    else
    {
        try
        {
            if ((u8 & 0xe0) == 0xc0 )
            {
                char32_t c = static_cast<char32_t>(u8) & 0x1f;
                return add_continuation(is, c);
            }
            else if ((u8 & 0xf0) == 0xe0)
            {
                char32_t c = static_cast<char32_t>(u8) & 0x0f;
                add_continuation(is, c);
                return add_continuation(is, c);
            }
            else if ((u8 & 0xf8) == 0xf0)
            {
                char32_t c = static_cast<char32_t>(u8) & 0x07;
                add_continuation(is, c);
                add_continuation(is, c);
                return add_continuation(is, c);
            }
        }
        catch (std::ios_base::failure)
        {
            if (is.eof())
                throw Invalid_character();
            throw;
        }
    }
    throw Invalid_character();
}

auto fauces::readline(istream& is, bool deletebom) -> u32string
{
    u32string line = u32string {};
    try
    {
        char32_t c = from_utf8(is);
        if (deletebom && c == 0xfeff)
            c = from_utf8(is);
        while (c != 0xa)
        {
            line += c;
            c = from_utf8(is);
        }
    }
    catch (std::ios_base::failure)
    {
        if (is.eof())
        {
            line += unicode_beot;
            return line;
        }
        throw;
    }
    return line;
}

char32_t fauces::universal(u32string text)
{
    // TODO: More context should be passed to this function.
    /*
     * * For example, \u0041 is valid inside a string, but invalid in an
     * * identifier.
     */
    if (text.size() > 0)
    {
        char32_t type = text[0];
        switch (type)
        {
            case U'u':
                if (text.size() != 5)
                    throw Invalid_universal();
                break;
            case U'U':
                if (text.size() != 9)
                    throw Invalid_universal();
                break;
            default:
                throw Invalid_universal();
        }
        char32_t code = 0;
        for (int i = 1; i < text.size(); ++i)
        {
            char32_t c = text[i];
            if (c >= U'0' && c <= U'9')
                c -= U'0';
            else if (c >= U'A' && c <= U'F')
                c -= U'A' - 10;
            else if (c >= U'a' && c <= U'f')
                c -= U'a' - 10;
            else
                throw Invalid_universal();
            code <<= 4;
            code |= c;
        }
        return code;
    }
    throw Invalid_universal();
}

auto fauces::plainchar_utf8(u32string text) -> string
{
    string utf8;
    for (auto c: text)
        utf8 += plainchar_utf8(c);
    return utf8;
}

auto fauces::plainchar_utf8(char32_t c) -> string
{
    string utf8;
    if (c <= 0x7f)
        utf8.push_back(plainchar(static_cast<unsigned char>(c)));
    else if (c <= 0x7ff)
    {
        utf8.push_back(plainchar(0xc0 | ((c >> 6) & 0x1f)));
        utf8.push_back(plainchar(0x80 | (c & 0x3f)));
    }
    else if (c <= 0xffff)
    {
        utf8.push_back(plainchar(0xe0 | ((c >> 12) & 0x0f)));
        utf8.push_back(plainchar(0x80 | ((c >> 6) & 0x3f)));
        utf8.push_back(plainchar(0x80 | (c & 0x3f)));
    }
    else
    {
        utf8.push_back(plainchar(0xf0 | ((c >> 18) & 0x07)));
        utf8.push_back(plainchar(0x80 | ((c >> 12) & 0x3f)));
        utf8.push_back(plainchar(0x80 | ((c >> 6) & 0x3f)));
        utf8.push_back(plainchar(0x80 | (c & 0x3f)));
    }
    return utf8;
}
