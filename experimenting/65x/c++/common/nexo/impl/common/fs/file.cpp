// file.cpp
// Hidden implementation of file-related classes
// Created by Alejandro Castro Garcia on 11 November 2020
/*
 Licensed under the MIT License
 
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


#include <nexo/file.hpp>

#include <iostream>
#include <unordered_map>

namespace nexo
{

using std::string;
using std::make_unique;
using std::unordered_map;
using std::out_of_range;

class File_info::Impl
{
    friend File_info;
private:
    unordered_map<string, string> attr;
};

File_info::File_info() : impl(make_unique<Impl>())
{
}

File_info::~File_info() = default; // Required by pimpl with unique_ptr

File_info& File_info::operator ^=(const File_info& mask)
{
// Remove attributes that are the same in this and mask
// and add attributes that are in mask but not in this.
    
    // Remove attributes that are the same in this and mask
    for (auto pair = impl->attr.begin(); pair != impl->attr.end();)
    {
        try
        {
            if (pair->second == mask.at(pair->first))
                pair = impl->attr.erase(pair);
            else
                ++pair;
                
        }
        catch (out_of_range)
        {
            pair = impl->attr.erase(pair);
        }
    }
    
    // Pending: add attributes that are in mask but not in this.
    return *this;
}

string& File_info::at(const string& key) const
{
    return impl->attr.at(key);
}

File_info::operator bool()
{
    return impl->attr.size();
}

class File_locator::Impl
{
    friend File_locator;
public:
    Impl(string ref) : path {ref}
    {
    }
private:
    string path;
};

File_locator::File_locator(string ref) : impl {make_unique<Impl>(ref)}
{}

File_locator::~File_locator() = default; // Required by pimpl with unique_ptr

} // namespace nexo
