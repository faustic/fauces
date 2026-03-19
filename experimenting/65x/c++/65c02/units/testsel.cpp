// Selection of test generator
//
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


#include "testsel.hpp"

#include "testgen/Hola_1.hpp"

#include <unordered_map>
#include <string>
#include <functional>

using std::string;
using std::unordered_map;
using std::function;
using std::unique_ptr;
using std::make_unique;

namespace w65c02
{

static unordered_map<string, function<unique_ptr<Test>(Mem&)>> fmap
{
    {"Hola_1", [](Mem& mem) {return make_unique<Hola_1>(mem);}}
};

unique_ptr<Test> named_test(const string& testname, Mem& mem)
{
    return fmap.at(testname)(mem);
}

} // namespace w65c02
