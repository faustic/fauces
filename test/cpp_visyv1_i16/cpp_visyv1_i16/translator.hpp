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

namespace fauces
{
class Translator: public Translated_unit_loader
{
public:
    Translator(const std::string& path) : path {path} {}
    static constexpr size_t max_include = 256;
private:
    const std::string path;
    std::unique_ptr<Translated_unit> load() override;
    
    static
        std::list<Token> preprocess(const std::string& path, size_t level = 0);
    static std::list<Token> pretokenize(const std::string& path);
};
}

#endif /* translator_hpp */
