//  basic_int.hpp
//  Utilities to deal with basic integer types
//
//  Created by Alejandro Castro García on 11 May 2020.
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

 Sometimes we need to deal with basic integer types in ways not supported
 directly by the standard library. We try to bring here the functions we need
 most often.

*/

#ifndef faulib_number_basic_int_hpp
#define faulib_number_basic_int_hpp

#include <type_traits>
#include <limits>

namespace faulib
{
namespace number
{

/*
** `bool fits_in_signed(unsigned_int_type)` **

This function returns true if the unsigned value fits in the
corresponding signed integer type. This can be useful if we want to convert
unsigned integers to signed integers and we want to avoid the
implementation-defined behaviour that such conversion may cause.
*/

template <typename unsigned_int_type>
bool fits_in_signed(unsigned_int_type n)
{
    using std::numeric_limits;
    if (std::is_unsigned<unsigned_int_type>::value)
    {
        using signed_int_type =
                            typename std::make_signed<unsigned_int_type>::type;
        return (n <= numeric_limits<signed_int_type>::max());
    }
    else if (std::is_signed<unsigned_int_type>::value) // Not useful, but allow it
        return (n >= 0);
    else
        return false;
}



/*
** `unsigned_int_type uabs(int_type)` **

Depending on the C++ implementation, the standard library function
`std::abs(int_type)` may not return a correct result. For example, if
`INT_MIN == -32768` and `INT_MAX == 32767`, then `std::abs(INT_MIN)` does not
fit in an `int` type and the call has undefined behaviour.

The function `uabs(int_type)` defined here returns a correct result for any
valid basic integer value, but it returns an unsigned type even if the original
argument was signed. So, if it is guaranteed that the original argument will
never be less than `-INT_TYPE_MAX`, it is generally better to use `std::abs`.
Otherwise, we should use `uabs` to prevent undefined behaviour.
*/
template <typename int_type>
typename std::make_unsigned<int_type>::type uabs(int_type original)
{
    if (std::is_unsigned<int_type>::value)
        return original;
    else
    {
// In C and C++, conversion to unsigned types works like two's complement even
// if the internal representation of signed integers is not two's complement.
        typename std::make_unsigned<int_type>::type abs_value = original;
        if (!fits_in_signed(abs_value))
            abs_value = ~abs_value + 1; // Negate (two's complement)
        return abs_value;
    }
}

/*
** `bool is_undefined_integer_add(int_type op1, int_type op2)` **

Adding signed integer types causes undefined behaviour if there is an overflow.
Overflows are usually avoided by choosing a big enough integer type that can
hold any possible value our program needs to use. However, sometimes we may
need to check at run time whether a particular operation will cause undefined
behaviour or not.
*/
template <typename int_type>
bool is_undefined_integer_add(int_type op1, int_type op2)
{
    using std::numeric_limits;
    if (numeric_limits<int_type>::is_signed) // Only signed overflow causes undefined behaviour
    {
        using uint_type = typename std::make_unsigned<int_type>::type;
        uint_type uop1 = op1;
        uint_type uop2 = op2;
        uint_type uresult = uop1 + uop2;
                
        if
        (
            (op1 > 0 && op2 > 0 && !fits_in_signed(uresult)) ||
            (op1 < 0 && op2 < 0 && fits_in_signed(uresult))
        )
            return true;
            
    }
    return false;
}

/*
** `bool is_undefined_integer_subtract(int_type op1, int_type op2)` **

Subtracting signed integer types causes undefined behaviour if there is an
overflow. Overflows are usually avoided by choosing a big enough integer type
that can hold any possible value our program needs to use. However, sometimes
we may need to check at run time whether a particular operation will cause
undefined behaviour or not.
*/
template <typename int_type>
bool is_undefined_integer_subtract(int_type op1, int_type op2)
{
    using std::numeric_limits;
    if (numeric_limits<int_type>::is_signed) // Only signed overflow causes undefined behaviour
    {
        using uint_type = typename std::make_unsigned<int_type>::type;
        uint_type uop1 = op1;
        uint_type uop2 = op2;
        uint_type uresult = uop1 - uop2;
                
        if
        (
            (op1 > 0 && op2 < 0 && !fits_in_signed(uresult)) ||
            (op1 < 0 && op2 > 0 && fits_in_signed(uresult))
        )
            return true;
            
    }
    return false;
}

/*
** `bool is_undefined_integer_multiply(int_type op1, int_type op2)` **

Multiplying signed integer types causes undefined behaviour if there is an
overflow. Overflows are usually avoided by choosing a big enough integer type
that can hold any possible value our program needs to use. However, sometimes
we may need to check at run time whether a particular operation will cause
undefined behaviour or not.
*/
template <typename int_type>
bool is_undefined_integer_multiply(int_type op1, int_type op2)
{
    using std::numeric_limits;
    if (numeric_limits<int_type>::is_signed) // Only signed overflow causes undefined behaviour
    {
        using uint_type = typename std::make_unsigned<int_type>::type;
        uint_type uop1 = uabs(op1);
        uint_type uop2 = uabs(op2);
          
        if (uop2 != 0)  // Overflow can only happen if both operands are
                        //non-zero.
        {
            uint_type uresult = uop1 * uop2;
            if
            (
                (uresult / uop2 != uop1) ||
                ((op1 < 0) == (op2 < 0) && !fits_in_signed(uresult)) ||
                ((op1 < 0) !=  (op2 < 0) && fits_in_signed(uresult))
            )
                return true;
        }
            
    }
    return false;
}

/*
** `bool is_undefined_integer_divide(int_type op1, int_type op2)` **

Dividing integer types causes undefined behaviour if the divisor is zero or if
there is an overflow. Overflows are usually avoided by choosing a big enough
integer type that can hold any possible value our program needs to use.
However, sometimes we may need to check at run time whether a particular
operation will cause undefined behaviour or not.
*/
template <typename int_type>
bool is_undefined_integer_divide(int_type op1, int_type op2)
{
    using std::numeric_limits;
    if (op2 == 0)
        return true;
    if (numeric_limits<int_type>::is_signed)    // Only signed division can
                                                // overflow
    {
        using uint_type = typename std::make_unsigned<int_type>::type;
        uint_type uop1 = uabs(op1);
        uint_type uop2 = uabs(op2);
          
        uint_type uresult = uop1 / uop2;
        if (((op1 < 0) &&  (op2 < 0) && !fits_in_signed(uresult)))
            return true;
            
    }
    return false;
}

} // number
} // faulib

#endif /* faulib_number_basic_int_hpp */
