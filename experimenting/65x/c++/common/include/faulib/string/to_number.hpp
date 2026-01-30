//  to_number.hpp
//  Conversions from string to number
//
//  Created by Alejandro Castro García on 12 May 2020.
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


#ifndef faulib_string_to_number_hpp
#define faulib_string_to_number_hpp

#include <string>
#include <sstream>


namespace faulib
{
namespace string
{

/*
 ** `number_type number_from_string(const std::string& str)` **

 This function returns a number from a string using a stringstream. If
 stringstream cannot convert the string into a number, zero is returned.

 */
template<typename number_type>
number_type number_from_string(const std::string& str)
{
    std::stringstream stream {str};
    number_type number;
    if (!( stream >> number))
        number = 0;
    return number;
}

} // string
} // faulib

#endif /* faulib_string_to_number_hpp */
