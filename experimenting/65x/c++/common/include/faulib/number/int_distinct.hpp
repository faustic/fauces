//  int_distinct.hpp
//  Type-safe integers
//
//  Created by Alejandro Castro Garcia on 11 August 2020.
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
  
 Sometimes we want an integer-like type that is equivalent to another integer
 or integer-like type but represents a distinct type that does not implicitly
 convert into other types.

 The types int_distinct defined here wrap an integer or integer-like type, and
 use an additional class as a marker parameter, so wrappers of the same integer
 type can represent different types.

*/

#ifndef faulib_number_int_distinct_hpp
#define faulib_number_int_distinct_hpp

namespace faulib
{
namespace number
{

template <typename int_type, typename marker>
class int_distinct
{
public:
    explicit int_distinct(int_type value) noexcept :
    value {value}
    {}

    int_distinct() = default; // To let this be a POD type

    explicit operator int_type() const noexcept
    {
        return value;
    }
    
    int_distinct operator+(int_distinct operand) const noexcept
    {
        return int_distinct(value + operand.value);
    }
    
    int_distinct operator-(int_distinct operand) const noexcept
    {
        return int_distinct(value - operand.value);
    }
    
    int_distinct operator*(int_distinct operand) const noexcept
    {
        return int_distinct(value * operand.value);
    }
    
    int_distinct operator/(int_distinct operand) const noexcept
    {
        return int_distinct(value / operand.value);
    }
    
    int_distinct operator%(int_distinct operand) const noexcept
    {
        return int_distinct(value % operand.value);
    }
    
    int_distinct operator|(int_distinct operand) const noexcept
    {
        return int_distinct(value | operand.value);
    }
    
    int_distinct operator&(int_distinct operand) const noexcept
    {
        return int_distinct(value & operand.value);
    }
    
    int_distinct operator^(int_distinct operand) const noexcept
    {
        return int_distinct(value ^ operand.value);
    }
    
    int_distinct operator>>(int operand) const noexcept
    {
        return int_distinct(value >> operand);
    }
    
    int_distinct operator<<(int operand) const noexcept
    {
        return int_distinct(value << operand);
    }
    
    int_distinct& operator+=(int_distinct operand) noexcept
    {
        value += operand.value;
        return *this;
    }
    
    int_distinct& operator-=(int_distinct operand) noexcept
    {
        value -= operand.value;
        return *this;
    }
    
    int_distinct& operator*=(int_distinct operand) noexcept
    {
        value *= operand.value;
        return *this;
    }
    
    int_distinct& operator/=(int_distinct operand) noexcept
    {
        value /= operand.value;
        return *this;
    }
    
    int_distinct& operator%=(int_distinct operand) noexcept
    {
        value %= operand.value;
        return *this;
    }
    
    int_distinct& operator|=(int_distinct operand) noexcept
    {
        value |= operand.value;
        return *this;
    }
    
    int_distinct& operator&=(int_distinct operand) noexcept
    {
        value &= operand.value;
        return *this;
    }
    
    int_distinct& operator^=(int_distinct operand) noexcept
    {
        value ^= operand.value;
        return *this;
    }
    
    int_distinct& operator>>=(int operand) noexcept
    {
        value >>= operand;
        return *this;
    }
    
    int_distinct& operator<<=(int operand) noexcept
    {
        value <<= operand;
        return *this;
    }
    
    int_distinct& operator++() noexcept
    {
        ++value;
        return *this;
    }
    
    int_distinct operator++(int) noexcept
    {
        int_distinct old {*this};
        ++value;
        return old;
    }
    
    int_distinct& operator--() noexcept
    {
        --value;
        return *this;
    }
    
    int_distinct operator--(int) noexcept
    {
        int_distinct old {*this};
        --value;
        return old;
    }
    
    int_distinct operator~() const noexcept
    {
        return int_distinct(~value);
    }
    
    bool operator==(int_distinct operand) const noexcept
    {
        return value == operand.value;
    }
    
    bool operator!=(int_distinct operand) const noexcept
    {
        return value != operand.value;
    }
    
    bool operator>(int_distinct operand) const noexcept
    {
        return value > operand.value;
    }
    
    bool operator>=(int_distinct operand) const noexcept
    {
        return value >= operand.value;
    }
    
    bool operator<(int_distinct operand) const noexcept
    {
        return value < operand.value;
    }
    
    bool operator<=(int_distinct operand) const noexcept
    {
        return value <= operand.value;
    }

private:
    int_type value;
}; // int_distinct

} // number
} // faulib


#endif /* faulib_number_int_distinct_hpp */
