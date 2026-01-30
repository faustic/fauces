// main.cpp
// Entry point to adc test program (currently just a playground to experiment)
//
// Created by Alejandro Castro Garcia on 27 November 2020
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

#include <climits>
#include <iostream>

using std::cout;

static unsigned add_uchar_bit(unsigned a, unsigned b, unsigned& carry)
{
// Sums one bit and puts carry into next position.
// For use by adc.
    static_assert(sizeof(unsigned) > sizeof(unsigned char));
    unsigned result = a ^ b ^ carry;
    carry = ((a & b) | (a & carry) | (b & carry)) << 1;
    return result;
}

static unsigned adc(unsigned char a, unsigned char b, bool& carry)
{
// Example of addition implementation.
// We are not allowed to use arithmetic operations.
// We are allowed to use bitwise operations with unsigned int objects.
// We are allowed to use basic conversions to and from unsigned int.
    static_assert(sizeof(unsigned) == sizeof(unsigned char) * 4);
    static_assert(CHAR_BIT == 8);
    unsigned carry_bit = static_cast<unsigned>(carry);
    unsigned mask = 1;
    unsigned result = add_uchar_bit(a & mask, b & mask, carry_bit);
    mask <<= 1;
    result |= add_uchar_bit(a & mask, b & mask, carry_bit);
    mask <<= 1;
    result |= add_uchar_bit(a & mask, b & mask, carry_bit);
    mask <<= 1;
    result |= add_uchar_bit(a & mask, b & mask, carry_bit);
    mask <<= 1;
    result |= add_uchar_bit(a & mask, b & mask, carry_bit);
    mask <<= 1;
    result |= add_uchar_bit(a & mask, b & mask, carry_bit);
    mask <<= 1;
    result |= add_uchar_bit(a & mask, b & mask, carry_bit);
    mask <<= 1;
    result |= add_uchar_bit(a & mask, b & mask, carry_bit);
    carry = static_cast<bool>(carry_bit);
    return result;
}

static unsigned add(unsigned a, unsigned b)
{
// Example of addition implementation.
// We are not allowed to use arithmetic operations, except adc.
// We are allowed to use bitwise operations with unsigned int objects.
// We are allowed to use basic conversions to and from unsigned int.
    static_assert(sizeof(unsigned) == sizeof(unsigned char) * 4);
    bool carry = 0;
    unsigned result = adc(a, b, carry);
    
    result |= adc(a >>= 8, b >>= 8, carry) << 8;
    result |= adc(a >>= 8, b >>= 8, carry) << 16;
    result |= adc(a >>= 8, b >>= 8, carry) << 24;
    return result;
}

static unsigned multiply(unsigned a, unsigned b)
{
// Example of multiplication implementation.
// We are not allowed to use arithmetic operations, except addition of unsigned
// int objects.
// We are allowed to use bitwise operations with unsigned int objects.
// We are allowed to use basic conversions to and from unsigned int.
    if (a < b)
    {
        unsigned c = a;
        a = b;
        b = c;
    }
    unsigned result = 0;
    while (b)
    {
        if (b & 1)
            result += a;
        a <<= 1;
        b >>= 1;
    }
    return result;
}

class TestFail
{};

int main()
{
    for (unsigned a = 0; a < 256; ++a)
    {
        for (unsigned b = 0; b < 256; ++b)
        {
            bool carry = false;
            unsigned result = adc(a, b, carry);
            unsigned expected = ((a + b) & 0xff);
            if (result == expected)
                cout << a << " + " << b << " == " << result <<
                                        (carry ? " plus carry" : "") << "\n";
            else
                throw TestFail();
        }
    }
    for (unsigned a = 0; a < 500; ++a)
    {
        for (unsigned b = 0; b < 500; ++b)
        {
            unsigned result = multiply(a, b);
            unsigned expected = a * b;
            if (result == expected)
                cout << a << " * " << b << " == " << result << "\n";
            else
                throw TestFail();
        }
    }
    for (unsigned a = 0; a < 500; ++a)
    {
        for (unsigned b = 0; b < 500; ++b)
        {
            unsigned result = add(a, b);
            unsigned expected = a + b;
            if (result == expected)
                cout << a << " + " << b << " == " << result << "\n";
            else
                throw TestFail();
        }
    }
}
