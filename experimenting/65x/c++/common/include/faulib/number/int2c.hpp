//  int2c.hpp
//  Type-safe, exact width, overflow-wrapped, two's complement signed integers
//
//  Created by Alejandro Castro García on 14 April 2020.
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

C++ implementations for most architectures support the signed types
`std::int8_t`, `std::int16_t`, `std::int32_t` and `std::int64_t`. When they are
available, it is guaranteed that they are exactly the indicated width and that
they have two's complement representation. These types are not fully portable,
as they are optional in the C++ standard. This is rarely a problem, because we
are not usually interested in a particular number of bits but rather in a
minimum range, so we can use the portable types `std::int_least8_t`,
`std::int_least16_t`, `std::int_least32_t` and `std::int_least64_t`.

However, sometimes we want an exact number of bits, such as when trying to
emulate foreign architectures or when parsing some particular kinds of
bitstreams. Often we also want signed integers to respond to operation overflow
in a way consistent through platforms, like basic unsigned integers do. In such
cases, using the standard signed integer types may lead to some problems:

* Traditionally, C++ signed integers were not guaranteed to have a two's
complement representation, so we could not directly map two's complement
integers from a bitstream. (Note: C++20 guarantees two's complement)
* Overflow is undefined. Depending on the implementation, a signed overflowing
operation can silently return a deterministic value like unsigned integers do,
or crash, or cause unpredictable behaviour. Not even the exact-width standard
integers guarantee predictable behaviour on overflow.
* Two standard integer types of different minimum width may actually be the
same type. For example, `int_least16_t` and `int_least32_t` may both be aliases of the `int` basic type. This may lead to duplicate functions that were
intended as overloading.

The types `int2c` defined here represent signed integers with an exact number
of bits, from 1 to 64. They are implemented as templates taking two parameters:
a basic unsigned integer type and an intended number of bits. The `int2c` type
keeps the passed basic type as its internal representation. We can make the
following guarantees, as long as the chosen basic unsigned integer type has at
least the chosen number of value bits:

* `int2c` types use a two's complement internal representation.
* `int2c` types with different number of intended bits are different types and
do not implicitly convert into each other.
* The maximum value of the N-bit `int2cN` type is exactly `pow(2, N-1) - 1`.
* The minimum value of the N-bit `int2cN` type is exactly `-pow(2, N-1)`.
* Overflows cause a wrap around that works consistently through C++
implementations like unsigned overflow does.

# POTENTIAL PROBLEMS
 
## Performance hit

Some operators applied to `int2c` are simply reapplied to their internal
representation, without further processing. In such cases, optimised compilers
will hopefully generate code as fast as if using the internal representation
directly. Unoptimised compilers may generate slower code.

Other operators may need to add an additional bitwise-and operation to restrict
the result to the maximum range. When the width of the `int2c` is the same as
that of the internal representation, optimised compilers may simply ignore the
bitwise-and operation, so hopefully there will not be any performance penalty
in this case either. When using a number of bits different that that of the
internal representation, code will be slower.

Yet other operators may need to deal with portable conversions between the
unsigned internal representation and the signed external appearance. These
operations are harder to optimise, so it is likely that the compiler will
generate much slower code than if we were using basic signed integer types.

## Compatibility issues

`int2c` is expected to be fully compatible with any C++17-compliant
implementation as long as the internal representation is a basic unsigned
integer type. Trying to use a basic signed integer type as the internal
representation will probably not work, because the overloaded operators assume
an unsigned type as the internal representation.

While compatibility of `int2c` with C++ implementations using rare widths for
their basic integer types is expected, it is unlikely to be ever verified in
practice, unless some user of such implementation gives it a try.
 
## Other

Any potential problem that can affect the types `faulib::number::uintx`, can
also probably affect the types `faulib::number::int2c` defined here.

*/

#ifndef faulib_number_int2c_hpp
#define faulib_number_int2c_hpp

#include <faulib/number/uintx.hpp>

#include <limits>
#include <cstdint>

namespace faulib
{
namespace number
{

template <typename basic_unsigned_type, int bits>
class int2c
{
public:
    using signed_type = typename std::make_signed<basic_unsigned_type>::type;
    
    explicit int2c(basic_unsigned_type value) noexcept :
    value {value}
    {}
    
    explicit int2c(uintx<basic_unsigned_type, bits> value) :
    value {value}
    {}

    int2c() = default; // To let this be a POD type
    
    explicit operator basic_unsigned_type() const noexcept
    {
// Here we convert to a basic unsigned type, which is a well-defined operation,
// although the returned value does not preserve width information.
        return value & mask;
    }
    
    explicit operator signed_type() const noexcept
    {
// Here we convert to a basic signed type. Automatic conversion from the basic
// unsigned internal representation can cause undefined behaviour, so we try to
// convert it through other means. If overflow happened because of padding
// bits in the signed type, it would be silently wrapped around.
        using type = std::numeric_limits<signed_type>;
        bool is_negative = value & sign_mask;
        if
        (
// In most systems type::min() can be represented as signed, but its absolute
// value can't. For example, abs(INT_MIN) can cause undefined behaviour. We
// deal with this case specially.
            type::min() < -type::max() &&
            type::digits == bits - 1 &&
            is_negative &&
            (value & positive_mask) == 0
        )
            return type::min();
        
// For all other cases we get the basic signed integer from the absolute
// value of the int2c.
        signed_type signed_value =
                    abs().value & positive_mask_signed; // Always well-defined
        return (is_negative ? -signed_value : signed_value);
    }
    
    explicit operator uintx<basic_unsigned_type, bits>() const noexcept
    {
// Conversion to uintx is a well-defined operation and also preserves width
// information
        return uintx<basic_unsigned_type, bits>(value);
    }
    
    int2c abs() const noexcept
    {
        return (value & sign_mask) ? -*this : *this;
    }

    int2c operator-() const noexcept
    {
        return int2c(~value + 1);
    }
    
    int2c operator+(int2c operand) const noexcept
    {
        return int2c(value + operand.value);
    }
    
    int2c operator-(int2c operand) const noexcept
    {
        return int2c(value - operand.value);
    }
    
    int2c operator*(int2c operand) const noexcept
    {
        bool sign = value & sign_mask;
        bool same_sign = sign == ((operand.value & sign_mask) != 0);
        basic_unsigned_type tmp = this->abs().value * operand.abs().value;

        return same_sign ? int2c(tmp) : -int2c(tmp);
    }
    
    int2c operator/(int2c operand) const noexcept
    {
        bool sign = value & sign_mask;
        bool same_sign = sign == ((operand.value & sign_mask) != 0);
        basic_unsigned_type tmp = this->abs().value / operand.abs().value;

        return same_sign ? int2c(tmp) : -int2c(tmp);
    }
    
    int2c operator%(int2c operand) const noexcept
    {
        bool sign = value & sign_mask;
        basic_unsigned_type tmp = this->abs().value % operand.abs().value;

        return sign ? -int2c(tmp) : int2c(tmp);
    }
    
    int2c operator|(int2c operand) const noexcept
    {
        return int2c(value | operand.value);
    }
    
    int2c operator&(int2c operand) const noexcept
    {
        return int2c(value & operand.value);
    }
    
    int2c operator^(int2c operand) const noexcept
    {
        return int2c(value ^ operand.value);
    }
    
    int2c operator>>(int operand) const noexcept
    {
    // This operation used to be undefined for C++ negative basic signed
    // integer types. We prefer to ensure that it works like an arithmetic
    // shift. Note: C++20 guarantees this behaviour for basic signed integers.
        
        if (value & sign_mask) // Negative number: invert before shifting and then invert again
            return int2c(~(~(value | ~mask) >> operand));
        else    // Positive number: shift directly (but mask before in case
                // bits is less than internal type size)
            return int2c((value & mask) >> operand);
    }
    
    int2c operator<<(int operand) const noexcept
    {
        return int2c(value << operand);
    }
    
    int2c& operator+=(int2c operand) noexcept
    {
        value += operand.value;
        return *this;
    }
    
    int2c& operator-=(int2c operand) noexcept
    {
        value -= operand.value;
        return *this;
    }
    
    int2c& operator*=(int2c operand) noexcept
    {
        bool sign = value & sign_mask;
        bool same_sign = sign == ((operand.value & sign_mask) != 0);
        value = this->abs().value * operand.abs().value;
        
        if (!same_sign)
            *this = -*this;
        
        return *this;
    }
    
    int2c& operator/=(int2c operand) noexcept
    {
        bool sign = value & sign_mask;
        bool same_sign = sign == ((operand.value & sign_mask) != 0);
        value = this->abs().value / operand.abs().value;

        if (!same_sign)
            *this = -*this;

        return *this;
    }
    
    int2c& operator%=(int2c operand) noexcept
    {
        bool sign = value & sign_mask;
        value = this->abs().value % operand.abs().value;
        
        if (sign)
            *this = -*this;
        
        return *this;
    }
    
    int2c& operator|=(int2c operand) noexcept
    {
        value |= operand.value;
        return *this;
    }
    
    int2c& operator&=(int2c operand) noexcept
    {
        value &= operand.value;
        return *this;
    }
    
    int2c& operator^=(int2c operand) noexcept
    {
        value ^= operand.value;
        return *this;
    }
    
    int2c& operator>>=(int operand) noexcept
    {
        // This operation used to be undefined for C++ negative basic signed
        // integer types. We prefer to ensure that it works like an arithmetic
        // shift. Note: C++20 already guarantees this for basic signed integers
        
        if (value & sign_mask) // Negative number: invert before shifting and then invert again
            value = ~(~(value | ~mask) >> operand);
        else    // Positive number: shift directly (but mask before in case
                // bits is less than internal type size)
            value = (value & mask) >> operand;

        return *this;
    }
    
    int2c& operator<<=(int operand) noexcept
    {
        value <<= operand;
        return *this;
    }
    
    int2c& operator++() noexcept
    {
        ++value;
        return *this;
    }
    
    int2c operator++(int) noexcept
    {
        int2c old {*this};
        ++value;
        return old;
    }
    
    int2c& operator--() noexcept
    {
        --value;
        return *this;
    }
    
    int2c operator--(int) noexcept
    {
        int2c old {*this};
        --value;
        return old;
    }
    
    int2c operator~() const noexcept
    {
        return int2c(~value);
    }
    
    bool operator==(int2c operand) const noexcept
    {
        return (value & mask) == (operand.value & mask);
    }
    
    bool operator!=(int2c operand) const noexcept
    {
        return (value & mask) != (operand.value & mask);
    }
    
    bool operator>(int2c operand) const noexcept
    {
        bool sign = value & sign_mask;
        bool same_sign = sign == ((operand.value & sign_mask) != 0);
        if (same_sign) // If same sign, compare values
            return (value & mask) > (operand.value & mask);
        else
            return !sign; // If different signs, this is greater if it is not negative
    }
    
    bool operator>=(int2c operand) const noexcept
    {
        bool sign = value & sign_mask;
        bool same_sign = sign == ((operand.value & sign_mask) != 0);
        if (same_sign) // If same sign, compare values
            return (value & mask) >= (operand.value & mask);
        else
            return !sign; // If different signs, this is greater if it is not negative
    }
    
    bool operator<(int2c operand) const noexcept
    {
        bool sign = value & sign_mask;
        bool same_sign = sign == ((operand.value & sign_mask) != 0);
        if (same_sign) // If same sign, compare values
            return (value & mask) < (operand.value & mask);
        else
            return sign; // If different signs, this is less if it is negative
    }
    
    bool operator<=(int2c operand) const noexcept
    {
        bool sign = value & sign_mask;
        bool same_sign = sign == ((operand.value & sign_mask) != 0);
        if (same_sign) // If same sign, compare values
            return (value & mask) <= (operand.value & mask);
        else
            return sign; // If different signs, this is less if it is negative
    }
    
    
private:
    
    basic_unsigned_type value;
    
// The following masks help us limit the range of the internal representation
    static constexpr basic_unsigned_type mask =
        std::numeric_limits<basic_unsigned_type>::max() >>
            (std::numeric_limits<basic_unsigned_type>::digits - bits);
    static constexpr basic_unsigned_type sign_mask =
        1ULL <<
            (std::numeric_limits<basic_unsigned_type>::digits - 1);
    static constexpr basic_unsigned_type positive_mask = mask & ~sign_mask;

// The following masks help us limit the range before conversion to a basic
// signed integer. Note that these masks are to be applied to the absolute
// value of the internal representation, not to the basic signed type.
    static constexpr basic_unsigned_type mask_signed = mask >>
                            (std::numeric_limits<basic_unsigned_type>::digits -
                                std::numeric_limits<signed_type>::digits - 1);
    static constexpr basic_unsigned_type sign_mask_signed =
        1ULL <<
            (std::numeric_limits<signed_type>::digits);
    static constexpr basic_unsigned_type positive_mask_signed =
                                            mask_signed & ~sign_mask_signed;

}; // class int2c

// Convenient portable aliases
using int2c2 = int2c<std::uint_least8_t, 2>;
using int2c3= int2c<std::uint_least8_t, 3>;
using int2c4 = int2c<std::uint_least8_t, 4>;
using int2c5 = int2c<std::uint_least8_t, 5>;
using int2c6 = int2c<std::uint_least8_t, 6>;
using int2c7 = int2c<std::uint_least8_t, 7>;
using int2c8 = int2c<std::uint_least8_t, 8>;
using int2c9 = int2c<std::uint_least16_t, 9>;
using int2c10 = int2c<std::uint_least16_t, 10>;
using int2c11 = int2c<std::uint_least16_t, 11>;
using int2c12 = int2c<std::uint_least16_t, 12>;
using int2c13 = int2c<std::uint_least16_t, 13>;
using int2c14 = int2c<std::uint_least16_t, 14>;
using int2c15 = int2c<std::uint_least16_t, 15>;
using int2c16 = int2c<std::uint_least16_t, 16>;
using int2c17 = int2c<std::uint_least32_t, 17>;
using int2c18 = int2c<std::uint_least32_t, 18>;
using int2c19 = int2c<std::uint_least32_t, 19>;
using int2c20 = int2c<std::uint_least32_t, 20>;
using int2c21 = int2c<std::uint_least32_t, 21>;
using int2c22 = int2c<std::uint_least32_t, 22>;
using int2c23 = int2c<std::uint_least32_t, 23>;
using int2c24 = int2c<std::uint_least32_t, 24>;
using int2c25 = int2c<std::uint_least32_t, 25>;
using int2c26 = int2c<std::uint_least32_t, 26>;
using int2c27 = int2c<std::uint_least32_t, 27>;
using int2c28 = int2c<std::uint_least32_t, 28>;
using int2c29 = int2c<std::uint_least32_t, 29>;
using int2c30 = int2c<std::uint_least32_t, 30>;
using int2c31 = int2c<std::uint_least32_t, 31>;
using int2c32 = int2c<std::uint_least32_t, 32>;
using int2c33 = int2c<std::uint_least64_t, 33>;
using int2c34 = int2c<std::uint_least64_t, 34>;
using int2c35 = int2c<std::uint_least64_t, 35>;
using int2c36 = int2c<std::uint_least64_t, 36>;

using int2c64 = int2c<std::uint_least64_t, 64>;


} // namespace number
} // namespace faulib

#endif /* faulib_number_int2c_hpp */
