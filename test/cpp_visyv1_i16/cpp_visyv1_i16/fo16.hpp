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

    std::unique_ptr<Translated_unit> load() override;
    void init(std::ifstream &ifs);
    bool load_section
                (std::istream& is, Translated_unit* unit, unsigned short id);
};

}

#endif /* fo16_hpp */
