//  uintx.hpp
//  Type-safe, exact width, overflow-wrapped unsigned integers
//
//  Created by Alejandro Castro García on 13 April 2020.
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

C++ implementations for most architectures support the types `std::uint8_t`,
`std::uint16_t`, `std::uint32_t` and `std::uint64_t`. These types are not fully
portable, as they are optional in the C++ standard. This is rarely a problem,
because we are not usually interested in a particular number of bits but rather
in a minimum range, so we can use the portable types `std::uint_least8_t`,
`std::uint_least16_t`, `std::uint_least32_t` and `std::uint_least64_t`.

However, sometimes we want an exact number of bits, such as when trying to
emulate foreign architectures or when parsing some particular kinds of
bitstreams. In such cases, using the standard minimum-width integer types may
lead to some problems:

* The minimum-width integer may not behave as an exact-width unsigned integer
would. For example, `{ uint_least8_t x {255}; ++x; }` may result in `x == 256`
in some C++ implementations, while `{ uint8_t x {255}; ++x };` would certainly
result in `x == 0`.
* Two standard integer types of different minimum width may actually be the
same type. For example, `uint_least16_t` and `uint_least32_t` may both be
aliases of the `unsigned int` basic type. This may lead to duplicate functions
that were intended as overloading.

The types `uintx` defined here represent unsigned integers with an exact number
of bits, from 1 to 64. They are implemented as templates taking two parameters:
a basic unsigned integer type and an intended number of bits. The `uintx` type
keeps the passed basic type as its internal representation. We can make the
following guarantees, as long as the chosen basic unsigned integer type has at
least the chosen number of value bits:

* `uintx` types with different number of intended bits are different types and
do not implicitly convert into each other.
* The maximum value of the N-bit `uintx` type is exactly `UINTN_MAX` as defined
by the C++ standard.
* Overflows work according to what the C++ standard mandates for unsigned
integer types: "Unsigned integers shall obey the laws of arithmetic modulo 2n".

# POTENTIAL PROBLEMS
 
## Performance hit

Some operators applied to `uintx` are simply reapplied to their internal
representation, without further processing. In such cases, optimised compilers
will hopefully generate code as fast as if using the internal representation
directly. Unoptimised compilers may generate slower code.

Other operators may need to add an additional bitwise-and operation to restrict
the result to the maximum range. When the width of the `uintx` is the same as
that of the internal representation, optimised compilers may simply ignore the
bitwise-and operation, so hopefully there will not be any performance penalty
in this case either. When using a number of bits different that that of the
internal representation, code will be slower.

## Compatibility issues

`uintx` is expected to be fully compatible with any C++17-compliant
implementation as long as the internal representation is a basic unsigned
integer type. Trying to use a basic signed integer type as the internal
representation will probably not work, except when the `uintx` type and the
internal representation have the same number of value bits. Even then, it may
not work in some implementations, because signed integers in C++ lack the
guarantee of obeying "the laws of arithmetic modulo 2n" (not anymore since
C++20).

While compatibility of `uintx` with C++ implementations using rare widths for
their basic integer types is expected, it is unlikely to be ever verified in
practice, unless some user of such implementation gives it a try.

## Other

Any potential problem that can affect the types `faulib::number::int_ts`, can
also probably affect the types `faulib::number::uintx` defined here.

*/

#ifndef faulib_number_uintx_hpp
#define faulib_number_uintx_hpp

#include <limits>
#include <cstdint>

namespace faulib
{
namespace number
{

template <typename basic_unsigned_type, int bits>
class uintx
{
public:
    
    explicit uintx(basic_unsigned_type value) noexcept :
    value {value}
    {}

    uintx() = default; // To let this be a POD type
    
    template<typename T, typename = typename std::enable_if<std::is_same<T, size_t>::value>::type>
    explicit uintx(T new_value) noexcept
    {
        value = basic_unsigned_type(new_value);
    }
    
    explicit operator basic_unsigned_type() const noexcept
    {
        return value & mask;
    }
    
    template <typename other_unsigned_type, int other_bits>
    explicit operator uintx<other_unsigned_type, other_bits>() const noexcept
    {
        return uintx<other_unsigned_type, other_bits>(
                            static_cast<other_unsigned_type>(value & mask));
    }
    
    template<typename T, typename = typename std::enable_if<std::is_same<T, size_t>::value>::type>
    explicit operator T() const noexcept
    {
        return static_cast<T>(value & mask);
    }
    
    uintx operator+(uintx operand) const noexcept
    {
        return uintx(value + operand.value);
    }
    
    uintx operator-(uintx operand) const noexcept
    {
        return uintx(value - operand.value);
    }
    
    uintx operator*(uintx operand) const noexcept
    {
        return uintx(value * operand.value);
    }
    
    uintx operator/(uintx operand) const noexcept
    {
        return uintx(value / operand.value);
    }
    
    uintx operator%(uintx operand) const noexcept
    {
        return uintx(value % operand.value);
    }
    
    uintx operator|(uintx operand) const noexcept
    {
        return uintx(value | operand.value);
    }
    
    uintx operator&(uintx operand) const noexcept
    {
        return uintx(value & operand.value);
    }
    
    uintx operator^(uintx operand) const noexcept
    {
        return uintx(value ^ operand.value);
    }
    
    uintx operator>>(int operand) const noexcept
    {
        return uintx((value & mask) >> operand);
    }
    
    uintx operator<<(int operand) const noexcept
    {
        return uintx(value << operand);
    }
    
    uintx& operator+=(uintx operand) noexcept
    {
        value += operand.value;
        return *this;
    }
    
    uintx& operator-=(uintx operand) noexcept
    {
        value -= operand.value;
        return *this;
    }
    
    uintx& operator*=(uintx operand) noexcept
    {
        value *= operand.value;
        return *this;
    }
    
    uintx& operator/=(uintx operand) noexcept
    {
        value /= operand.value;
        return *this;
    }
    
    uintx& operator%=(uintx operand) noexcept
    {
        value %= operand.value;
        return *this;
    }
    
    uintx& operator|=(uintx operand) noexcept
    {
        value |= operand.value;
        return *this;
    }
    
    uintx& operator&=(uintx operand) noexcept
    {
        value &= operand.value;
        return *this;
    }
    
    uintx& operator^=(uintx operand) noexcept
    {
        value ^= operand.value;
        return *this;
    }
    
    uintx& operator>>=(int operand) noexcept
    {
        value &= mask;
        value >>= operand;
        return *this;
    }
    
    uintx& operator<<=(int operand) noexcept
    {
        value <<= operand;
        return *this;
    }
    
    uintx& operator++() noexcept
    {
        ++value;
        return *this;
    }
    
    uintx operator++(int) noexcept
    {
        uintx old {*this};
        ++value;
        return old;
    }
    
    uintx& operator--() noexcept
    {
        --value;
        return *this;
    }
    
    uintx operator--(int) noexcept
    {
        uintx old {*this};
        --value;
        return old;
    }
    
    uintx operator~() const noexcept
    {
        return uintx(~value);
    }
    
    bool operator==(uintx operand) const noexcept
    {
        return (value & mask) == (operand.value & mask);
    }
    
    bool operator!=(uintx operand) const noexcept
    {
        return (value & mask) != (operand.value & mask);
    }
    
    bool operator>(uintx operand) const noexcept
    {
        return (value & mask) > (operand.value & mask);
    }
    
    bool operator>=(uintx operand) const noexcept
    {
        return (value & mask) >= (operand.value & mask);
    }
    
    bool operator<(uintx operand) const noexcept
    {
        return (value & mask) < (operand.value & mask);
    }
    
    bool operator<=(uintx operand) const noexcept
    {
        return (value & mask) <= (operand.value & mask);
    }
    
    
private:
    
    basic_unsigned_type value;
    static constexpr basic_unsigned_type mask =
        std::numeric_limits<basic_unsigned_type>::max() >>
            (std::numeric_limits<basic_unsigned_type>::digits - bits);

}; // class uintx

// Convenient portable aliases
using uintx1 = uintx<std::uint_least8_t, 1>;
using uintx2 = uintx<std::uint_least8_t, 2>;
using uintx3= uintx<std::uint_least8_t, 3>;
using uintx4 = uintx<std::uint_least8_t, 4>;
using uintx5 = uintx<std::uint_least8_t, 5>;
using uintx6 = uintx<std::uint_least8_t, 6>;
using uintx7 = uintx<std::uint_least8_t, 7>;
using uintx8 = uintx<std::uint_least8_t, 8>;
using uintx9 = uintx<std::uint_least16_t, 9>;
using uintx10 = uintx<std::uint_least16_t, 10>;
using uintx11 = uintx<std::uint_least16_t, 11>;
using uintx12 = uintx<std::uint_least16_t, 12>;
using uintx13 = uintx<std::uint_least16_t, 13>;
using uintx14 = uintx<std::uint_least16_t, 14>;
using uintx15 = uintx<std::uint_least16_t, 15>;
using uintx16 = uintx<std::uint_least16_t, 16>;
using uintx17 = uintx<std::uint_least32_t, 17>;
using uintx18 = uintx<std::uint_least32_t, 18>;
using uintx19 = uintx<std::uint_least32_t, 19>;
using uintx20 = uintx<std::uint_least32_t, 20>;
using uintx21 = uintx<std::uint_least32_t, 21>;
using uintx22 = uintx<std::uint_least32_t, 22>;
using uintx23 = uintx<std::uint_least32_t, 23>;
using uintx24 = uintx<std::uint_least32_t, 24>;
using uintx25 = uintx<std::uint_least32_t, 25>;
using uintx26 = uintx<std::uint_least32_t, 26>;
using uintx27 = uintx<std::uint_least32_t, 27>;
using uintx28 = uintx<std::uint_least32_t, 28>;
using uintx29 = uintx<std::uint_least32_t, 29>;
using uintx30 = uintx<std::uint_least32_t, 30>;
using uintx31 = uintx<std::uint_least32_t, 31>;
using uintx32 = uintx<std::uint_least32_t, 32>;
using uintx33 = uintx<std::uint_least64_t, 33>;
using uintx34 = uintx<std::uint_least64_t, 34>;
using uintx35 = uintx<std::uint_least64_t, 35>;
using uintx36 = uintx<std::uint_least64_t, 36>;

using uintx64 = uintx<std::uint_least64_t, 64>;


} // namespace number
} // namespace faulib


#endif /* faulib_number_uintx_hpp */
