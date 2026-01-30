// phase7.cpp
// Phase 7 of translation
// Intended compatibility: c++17
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


#include "phase7.hpp"

#include "translator.hpp"

#include <iostream>

// Phase 7 requires full implementation of Translator::analyze

void fauces::remove_white_space(list<Token>& tokens)
{
    auto t = tokens.begin();
    while (t != tokens.end())
    {
        if ((*t).type == Token_type::white)
            t = tokens.erase(t);
        else
            ++t;
    }
}

void fauces::bad_token(size_t index, const Token& t)
{
    std::cerr << index << ". Unknown: " << t.text << "\n";
}

