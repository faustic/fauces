// arch.hpp
// Types for virtual system CPU programmer's model
// Intended compatibility: c++17
//
// Created by Alejandro Castro Garcia on 28 December 2020
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


#ifndef fauvisy_arch_hpp
#define fauvisy_arch_hpp

namespace vs
{
class Register
{
};

class R : public Register
{
public:
    explicit R(unsigned n) : n {n & 1}
    {}
    operator unsigned() const noexcept
    {
        return n;
    }
private:
    unsigned n;
};

class S : public Register
{
public:
    explicit S(unsigned n) : n {n & 1}
    {}
    operator unsigned()
    {
        return n;
    }
private:
    unsigned n;
};

class X : public Register
{
public:
    explicit X(unsigned n) : n {n & 1}
    {}
    operator unsigned()
    {
        return n;
    }
private:
    unsigned n;
};

class Imme
{
public:
    explicit Imme(unsigned n) : n {n & 0xf}
    {}
    operator unsigned()
    {
        return n;
    }
private:
    unsigned n;
};


} // namespace vs

#endif /* fauvisy_arch_hpp */
