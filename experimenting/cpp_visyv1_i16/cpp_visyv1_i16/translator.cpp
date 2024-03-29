// Management of the translation process
// Intended compatibility: c++20
//
// Created by Alejandro Castro Garcia on 23 September 2021
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


#include "translator.hpp"

#include "phase4.hpp"

#include "../../visy1010/visy1010/using_iostream.hpp"
#include "../../visy1010/visy1010/using_containers.hpp"
#include "../../visy1010/visy1010/using_string.hpp"
#include "../../visy1010/visy1010/using_memory.hpp"
#include "../../visy1010/visy1010/using_algorithm.hpp"

auto fauces::Translator::preprocess(const string& path, size_t level)
    -> list<Token>
{
    if (level > max_include)
        throw Limit_error {"Included file is too nested"};
    list<Token> tokens = pretokenize(path);
    execute_directives(tokens, level);
    return tokens;
}

unique_ptr<fauces::Translated_unit> fauces::Translator::load()
{
    list<Token> tokens = preprocess(path);
    convert_literals(tokens);
    concatenate_literals(tokens);
    auto unit = make_unique<Translated_unit>();
    analyze(tokens, *unit);
    instantiate(*unit);
    return unit;
}
