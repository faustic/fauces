//  int_ts.hpp
//  Type-safe, unspecified width, overflow-undefined integers
//
//  Created by Alejandro Castro García on 7 April 2020.
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
 
Basic integer types can be implicitily converted into other basic integer
types. This is often convenient. However, it can sometimes cause unintended
effects, like:
 
* Ambiguities when calling overloaded functions.
* Loss of information when inadvertently converting to a smaller type (although
 hopefully the compiler will warn about this).

The types int_ts defined here simply wrap the basic integer types so that they
can be used without implicit conversions. We intend to redefine all or most of
the arithmetic operators, and the result will not be converted into a different
type unless explicitly requested.

# POTENTIAL PROBLEMS
 
## Performance hit

Unoptimised compilers may generate slower code when using the int_ts types.
Hopefully, optimised compilers will generate code as fast as if using basic
integer types, as all operators are defined inline.
 
## Explicit conversions

While implicit conversions are more dangerous, too many explicit conversions
can quickly become tedious and annoying, and they can make the code more
difficult to read.

Theoretically, if we consistently use only the int_ts types through all our
code, we will not have much need of explicit conversions. In practice we may
often need to use external functions that expect basic integer types instead of
our wrappers. Our code may look better if we make overloaded versions of the
external functions we use most often, so that most explicit conversions are
made in just a few places.

*/

#ifndef faulib_number_int_ts_hpp
#define faulib_number_int_ts_hpp

#include <faulib/number/int_distinct.hpp>

namespace faulib
{
namespace number
{

template <typename int_type>
using int_ts = int_distinct<int_type, int_type>;


} // namespace number
} // namespace faulib


#endif /* faulib_number_int_ts_hpp */
