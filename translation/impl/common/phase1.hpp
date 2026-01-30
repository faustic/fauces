// phase1.hpp
// Phase 1 of translation
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


#ifndef phase1_hpp
#define phase1_hpp

#include <string>
#include <iostream>
#include <stdexcept>

namespace fauces
{
using std::istream;
using std::string;
using std::u32string;

enum: char32_t
{
    unicode_eot = 4,
    unicode_beot = (char32_t)(-1) // Extraofficial EOT for binary transmissions
};

class Invalid_character: std::runtime_error
{
public:
    Invalid_character() : std::runtime_error("Invalid character")
    {}
};

class Invalid_universal: std::runtime_error
{
public:
    Invalid_universal(): std::runtime_error("Invalid universal character name")
    {}
};

u32string readline(istream& is, bool deletebom = false);

char32_t universal(u32string text);

string plainchar_utf8(u32string text);
string plainchar_utf8(char32_t c);

}

#endif /* phase1_hpp */
