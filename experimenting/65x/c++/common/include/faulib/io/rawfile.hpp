// rawfile.hpp
// Frequently performed tasks with files and streams
//
// Created by Alejandro Castro García on 8 September 2021
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


/*
 # EXPLANATION OF INTENT

 Functions to read and write raw binary data.

*/



#ifndef faulib_rawfile_hpp
#define faulib_rawfile_hpp

#include <span>
#include <vector>
#include <fstream>
#include <iostream>
#include <limits>

namespace faulib::io
{

using std::vector;
using std::ostream;
using std::istream;
using std::ofstream;
using std::ifstream;
using std::span;
using std::streamsize;
using std::streampos;
using std::numeric_limits;

template<typename T>
ostream& writeraw(ostream& os, span<T> rawdata)
{
    char* ptr = reinterpret_cast<char*>(rawdata.data());
    size_t size = rawdata.size() * sizeof(T);
    static constexpr streamsize max = numeric_limits<streamsize>::max();
    while (size > max)
    {
        os.write(ptr, max);
        if (!os.good())
            return os;
        size -= static_cast<size_t>(max);
        ptr += static_cast<size_t>(max);
    }
    if (size)
        os.write(ptr, size);
    return os;
}

template<typename T>
void writeraw(const char* path, span<T> rawdata)
{
    ofstream ofs;
    ofs.exceptions(ofs.failbit | ofs.badbit);
    ofs.open(path, ofs.binary | ofs.trunc);
    writeraw(ofs, rawdata);
    ofs.close();
}

template<typename T>
istream& readraw(istream &is, span<T> rawdata)
{
    char* ptr = reinterpret_cast<char*>(rawdata.data());
    size_t size = rawdata.size() * sizeof(T);
    static constexpr streamsize max = numeric_limits<streamsize>::max();
    while (size > max)
    {
        is.read(ptr, max);
        if (!is.good())
            return is;
        size -= static_cast<size_t>(max);
        ptr += static_cast<size_t>(max);
    }
    if (size)
         is.read(ptr, size);
    return is;
}

template<typename T>
vector<T> readraw(istream &is, typename vector<T>::size_type n)
{
    vector<T> rawdata(n);
    readraw<T>(is, rawdata);
    return rawdata;
}

inline streampos get_stream_size(istream &is)
{
    streampos old_pos = is.tellg();
    is.seekg(0, is.end);
    streampos n = is.tellg();
    is.seekg(old_pos, is.beg);
    return n;
}


template<typename T>
vector<T> readraw(const char* path)
{
    ifstream ifs;
    ifs.exceptions(ifs.failbit | ifs.badbit | ifs.eofbit);
    ifs.open(path, ifs.binary);
    streampos n = get_stream_size(ifs) / sizeof(T);
    using Size = typename vector<T>::size_type;
    if (n > numeric_limits<Size>::max())
        n = numeric_limits<Size>::max();
    vector<T> rawdata = readraw<T>(ifs, static_cast<Size>(n));
    ifs.close();
    return rawdata;
}

} // namespace faulib::io

#endif /* faulib_rawfile_hpp */
