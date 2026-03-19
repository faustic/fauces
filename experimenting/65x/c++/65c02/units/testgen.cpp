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
#include "executable.hpp"

#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace w65c02
{
class Hola_1: public Test
{
public:
    Hola_1(Mem& mem): Test(mem)
    {
        as.lda(Imm(0x48));
        as.sta(Abs(0x3000));
        as.lda(Imm(0x6f));
        as.sta(Abs(0x3001));
        as.lda(Imm(0x6c));
        as.sta(Abs(0x3002));
        as.lda(Imm(0x61));
        as.sta(Abs(0x3003));
        as.rts();
        end();
    }
private:
    Address result_start()
    {
        return 0x3000;
    }
    
    size_t result_size()
    {
        return 4;
    }
};
}

using std::string;
using std::cout;
using std::unique_ptr;
using std::make_unique;
using std::runtime_error;
using std::filesystem::create_directory;
using namespace w65c02;

class Test_error : public runtime_error
{
public:
    Test_error(const std::string& what_arg) : runtime_error(what_arg)
    {
    }
};

unique_ptr<w65c02::Test> named_test(std::string testname, Mem& mem)
{
    if (testname != "Hola_1")
        throw Test_error("Unknown test name");
    return make_unique<w65c02::Hola_1>(mem);
}

int main(int argc, char** argv)
{
    cout << "TEST GENERATION\n";
    cout << (argc - 1) << "\n";
    Mem_le_plain mem;
    for (int i = 1; i < argc; ++i)
    {
        cout << argv[i] << "\n";
        auto test = named_test(argv[i], mem);
        cout << "Code start: " << test->code_start() << "\n";
        cout << "Code size: " << test->code_size() << "\n";
        cout << "Result start: " << test->result_start() << "\n";
        cout << "Result size: " << test->result_size() << "\n";
        cout << "\n";
        if (test->code_size() > 4076)
            throw Test_error("Code too big");
        Executable exec(mem, test->code_start(), test->code_size(),
                        test->result_start(), test->result_size());
        string dir_name = argv[1];
        create_directory(dir_name);
        exec.save(dir_name + "/TEST");
    }
    return 0;
}

