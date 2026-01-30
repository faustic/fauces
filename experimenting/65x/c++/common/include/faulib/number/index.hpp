//
//  index.hpp
//
//  Created by Alejandro Castro Garcia on 15 September 2020.
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

The types generally used as indexes of arrays are `ptrdiff_t` and `size_t`. The
latter is the one typically used as the argument to the operator [] in standard
containers. However, sometimes we may want to use a different type as an index.

The `index` template defined here acts simply as a name for index-like objects
and as a wrapper for an internal representation to be defined by the user of
the template.
 
An `offset` template is also defined here representing the difference between
two `index` objects. We define `offset` as a type different from `index` to
allow for a stricter logic: the difference between two indexes should not
necessarily make sense as an index.

The operator [] will not be overloaded here, but in the container-like class
that will use the index.


*/
#ifndef faulib_number_index_hpp
#define faulib_number_index_hpp

#include <cstddef>
#include <type_traits>  

namespace faulib
{
namespace number
{

template <typename internal_type, typename marker>
class index
{
public:
    explicit index(internal_type value) noexcept:
    private_value {value}
    {}
    
    index() = default; // To let this be a POD type
    
    template<typename T, typename = typename std::enable_if<std::is_same<T, size_t>::value>::type>
    explicit index(T value) noexcept
    {
        private_value = internal_type(value);
    }
    
    explicit operator internal_type() const noexcept
    {
        return private_value;
    }
    
    template<typename T, typename = typename std::enable_if<std::is_same<T, size_t>::value>::type>
    explicit operator T() const noexcept
    {
        return static_cast<T>(private_value);
    }
    
    index& operator++() noexcept
    {
        ++private_value;
        return *this;
    }
    
    index operator++(int) noexcept
    {
        index old {*this};
        ++private_value;
        return old;
    }
    
    index& operator--() noexcept
    {
        --private_value;
        return *this;
    }
    
    index operator--(int) noexcept
    {
        index old {*this};
        --private_value;
        return old;
    }
    
    bool operator==(index operand) const noexcept
    {
        return private_value == operand.private_value;
    }
    
    bool operator!=(index operand) const noexcept
    {
        return private_value != operand.private_value;
    }
    
    bool operator>(index operand) const noexcept
    {
        return private_value > operand.private_value;
    }
    
    bool operator>=(index operand) const noexcept
    {
        return private_value >= operand.private_value;
    }
    
    bool operator<(index operand) const noexcept
    {
        return private_value < operand.private_value;
    }
    
    bool operator<=(index operand) const noexcept
    {
        return private_value <= operand.private_value;
    }

private:
    internal_type private_value;
}; // index

template <typename internal_type, typename marker>
class offset
{
public:
    explicit offset(internal_type value) noexcept:
    private_value {value}
    {}
    
    offset() = default; // To let this be a POD type
    
    explicit operator internal_type() const noexcept
    {
        return private_value;
    }
    
    template<typename T, typename = typename std::enable_if<std::is_same<T, size_t>::value>::type>
    explicit operator T() const noexcept
    {
        return static_cast<T>(private_value);
    }
    
    offset operator+(offset offset2) const noexcept
    {
        return offset(private_value + internal_type(offset2));
    }
    
    offset operator-(offset offset2) const noexcept
    {
        return offset(private_value - internal_type(offset2));
    }
    
    offset operator+(index<internal_type, marker> index2) const noexcept
    {
        return offset(private_value + index<internal_type,
                                                            marker>(index2));
    }
    
    offset operator-(index<internal_type, marker> index2) const noexcept
    {
        return offset(private_value - index<internal_type,
                                                            marker>(index2));
    }

    offset& operator-=(offset operand) noexcept
    {
        private_value -= operand.private_value;
        return *this;
    }
    
    offset& operator++() noexcept
    {
        ++private_value;
        return *this;
    }
    
    offset operator++(int) noexcept
    {
        offset old {*this};
        ++private_value;
        return old;
    }
    
    offset& operator--() noexcept
    {
        --private_value;
        return *this;
    }
    
    offset operator--(int) noexcept
    {
        offset old {*this};
        --private_value;
        return old;
    }
    
    bool operator==(offset operand) const noexcept
    {
        return private_value == operand.private_value;
    }
    
    bool operator!=(offset operand) const noexcept
    {
        return private_value != operand.private_value;
    }
    
    bool operator>(offset operand) const noexcept
    {
        return private_value > operand.private_value;
    }
    
    bool operator>=(offset operand) const noexcept
    {
        return private_value >= operand.private_value;
    }
    
    bool operator<(offset operand) const noexcept
    {
        return private_value < operand.private_value;
    }
    
    bool operator<=(offset operand) const noexcept
    {
        return private_value <= operand.private_value;
    }
private:
    internal_type private_value;
}; // offset

template <typename internal_type, typename marker>
offset<internal_type, marker> operator-(index<internal_type, marker> index1,
                                index<internal_type, marker> index2) noexcept
{
    return offset<internal_type, marker>(internal_type(index1) -
                                                        internal_type(index2));
}

template <typename internal_type, typename marker>
index<internal_type, marker> operator+(index<internal_type, marker> index1,
                                offset<internal_type, marker> offset2) noexcept
{
    return index<internal_type, marker>(internal_type(index1) +
                                                    internal_type(offset2));
}

template <typename internal_type, typename marker>
index<internal_type, marker> operator-(index<internal_type, marker> index1,
                                        offset<internal_type, marker> offset2)
{
    return index<internal_type, marker>(internal_type(index1) -
                                                    internal_type(offset2));
}

} // number
} // faulib

#endif /* faulib_number_index_hpp */
