// file.hpp
// File-related classes
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



#ifndef nexo_file_hpp
#define nexo_file_hpp

#include <string>
#include <memory>
#include <vector>

namespace nexo
{

class Error_bad_key
{
public:
    Error_bad_key(std::string key) : priv_key {key}
    {}
    const std::string& key() const
    {
        return priv_key;
    }
private:
    std::string priv_key;
};

class File_info
{
public:
    File_info();
    ~File_info();
    std::string& at(const std::string& key) const;
    operator bool();
    File_info& operator ^=(const File_info&);
private:
    class Impl;
    std::unique_ptr<Impl> impl;
}; // class File_info

class File_locator
{
public:
    File_locator(std::string ref);
    ~File_locator();
    
    File_info get_info(std::vector<std::string> keys);
    
private:
    class Impl;
    std::unique_ptr<Impl> impl;
}; // class File_locator

class File_pass
{
public:
private:
}; // class File_pass

} // namespace nexo

#endif /* nexo_file_hpp */
