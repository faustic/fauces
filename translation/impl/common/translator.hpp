// A class to manage the translation process
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


#ifndef translator_hpp
#define translator_hpp

#include "pieces.hpp"
#include <list>
#include <string>
#include <memory>

namespace fauces
{
using std::list;
using std::string;
using std::unique_ptr;

void remove_white_space(list<Token>& tokens);
void bad_token(size_t index, const Token& token);

class Preprocessor
{
public:
    static constexpr size_t max_include = 256;
    static list<Token> preprocess(const string& path, size_t level = 0);
private:
    static std::list<Token> pretokenize(const string& path);
    static void execute_directives(list<Token>& tokens, size_t level = 0);
    static void convert_literals(list<Token>& tokens);
    static void concatenate_literals(list<Token>& tokens);
};

template<typename Arch>
class Translator: public Translated_unit_loader
{
public:
    Translator(const string& path) : path {path} {}
private:
    const string path;
    unique_ptr<Translated_unit> load() override
    {
        list<Token> tokens = preprocess(path);
        auto unit = make_unique<Translated_unit>();
        analyze(tokens, *unit);
        instantiate(*unit);
        return unit;
    }
    static list<Token> preprocess(const string& path, size_t level = 0)
    {
        return Preprocessor::preprocess(path, level);
    }
    
    static void analyze(list<Token>& tokens, Translated_unit& unit)
    {
        size_t n = 0;
        remove_white_space(tokens);
        for (auto& t: tokens)
        {
            if (t.type == Token_type::unknown)
                bad_token(n, t);
            ++n;
        }
        throw Syntax_error {"No syntax defined yet: everything is an error"};
    }

    static void instantiate(Translated_unit& unit)
    {
        // Templates not supported yet.
        // For the time being, this acts like a no-op.
    }
};
}

#endif /* translator_hpp */
