//  span.hpp
//  Span-like class template for C++ older than C++20
//
//  Created by Alejandro Castro Garcia on 29/07/2020.
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

std::span only exists since C++20. For older implementations we may want to use
an alternative class of our own. It does not need to be fully compatible with
the standard version, it just needs to cover our typical needs.

*/

#ifndef faulib_pre_std_span_h
#define faulib_pre_std_span_h

#include <vector>
#include <iterator>
#include <stdexcept>

namespace faulib
{
namespace pre_std
{

template <class T>
class span
{
public:
    using element_type = T;
    using value_type = typename std::remove_cv_t<T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
    using reverse_iterator = typename std::reverse_iterator<iterator>;
    
    span() :
    first {nullptr},
    count {0}
    {
    }
    
    template <class It>
    span(It first, size_type count) noexcept :
    first {&*first},
    count {count}
    {}
    
    template <class It, class End>
    span(It first, End last) noexcept :
    first {&*first},
    count {last - first}
    {}
    
    iterator begin() const noexcept
    {
        return first;
    }
    
    iterator end() const noexcept
    {
        return first + count;
    }
    
    reverse_iterator rbegin() const noexcept
    {
        return reverse_iterator(end());
    }
    
    reverse_iterator rend() const noexcept
    {
        return reverse_iterator(begin());
    }
    
    reference front() const noexcept
    {
        return *begin();
    }
    
    reference back() const noexcept
    {
        return *(end() - 1);
    }
    
    reference operator[](size_type idx) const noexcept
    {
        return data()[idx];
    }
    
    pointer data() const noexcept
    {
        return first;
    }
    
    size_type size() const noexcept
    {
        return count;
    }
    
    size_type size_bytes() const noexcept
    {
        return size() * sizeof(element_type);
    }
    
    bool empty() const noexcept
    {
        return count == 0;
    }

private:
    pointer first;
    size_type count;
    
}; // class span


} // namespace pre_std
} // namespace faulib

#endif /* faulib_pre_std_span_h */
