// files.cpp
// File transformation.
//
// Created by Alejandro Castro García on 15 April 2021
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


#include "files.hpp"


namespace fauces
{


static File_type identify_source_file(string filename)
{
    return File_type::cpp;
}

File_type identify_file_type(string filename)
{
    using std::ios;
    std::ifstream ifs;
    ifs.exceptions(ios::failbit | ios::badbit | ios::eofbit);
    try
    {
        ifs.open(filename, ios::binary);
    }
    catch (...)
    {
        throw File_error_cantopen();
    }
    try
    {
        array<unsigned char, 16> signature;
        read(ifs, signature.data(), signature.size());
        if (Fo16_unit_loader::is_signature(signature))
            return File_type::fo16;
    }
    catch (...)
    {
        if (!ifs.eof())
            throw File_error_read();
    }
    return identify_source_file(filename);
}


} // namespace fauces
