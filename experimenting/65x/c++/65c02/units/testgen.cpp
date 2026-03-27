// Generate test programs for the 65c02 emulator
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

#include "testgen.hpp"
#include "testsel.hpp"
#include "executable.hpp"

#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>

using std::string;
using std::cout;
using std::cerr;
using std::unique_ptr;
using std::make_unique;
using std::runtime_error;
using std::filesystem::create_directory;
using namespace w65c02;

static void generate_named_test(const string& testname)
{
    Mem_le_plain mem;
    cout << testname << "\n";
    auto test = named_test(testname, mem);
    cout << "Code start: " << test->code_start() << "\n";
    cout << "Code size: " << test->code_size() << "\n";
    cout << "Result start: " << test->result_start() << "\n";
    cout << "Result size: " << test->result_size() << "\n";
    cout << "\n";
    if (test->code_size() > 4076)
        throw Test_error("Code too big");
    Executable exec(mem, test->code_start(), test->code_size(),
                    test->result_start(), test->result_size());
    string dir_name = testname;
    create_directory(dir_name);
    exec.save(dir_name + "/TEST");
}

static void generate_named_tests(int argc, char** argv)
{
    cout << (argc - 1) << "\n";
    for (int i = 1; i < argc; ++i)
        generate_named_test(argv[i]);
}

static void generate_all_tests()
{
    cout << "ALL TESTS\n";
    start_tests();
    for (auto name = next_test(); name != ""; name = next_test())
        generate_named_test(name);
    
}

int main(int argc, char** argv)
{
    cout << "TEST GENERATION\n";
    if (argc == 2 && string(argv[1]) == "--all")
        generate_all_tests();
    else if (argc < 2)
    {
        cerr << "You need to pass arguments";
        cerr << ", either --all or valid test names\n";
    }
    else
        generate_named_tests(argc, argv);
    return 0;
}

