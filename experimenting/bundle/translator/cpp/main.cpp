// main.cpp
// Experimenting with compiler concepts
// Intended compatibility: c++17
//
// Created by Alejandro Castro García on 9 February 2021
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

#include "files.hpp"

namespace fauces
{
    struct Arg_handle
    {
        int pos;
        int argc;
        char** argv;
        unordered_map<string, string> options;
    };

    struct Program_arg
    {
        vector<Program_input> inputs;
        Program_output output;
    };

    unordered_map<char, string> expanded = {{'o', "output"}};
    unordered_map<string, bool> expected_empty = {{"output", false}};

    bool parse_options(Arg_handle& harg)
    {
        string item {harg.argv[harg.pos]};
        try
        {
            if (item.at(0) == '-')
            {
                char c = item.at(1);
                string key;
                if (c == '-')
                    key = item.substr(2);
                else
                {
                    key = expanded.at(c);
                    if (item.size() > 2)
                    {
                        if (expected_empty.at(key))
                            return false;
                        else
                        {
                            harg.options[key] = item.substr(2);
                            ++harg.pos;
                            return true;
                        }
                    }
                }
                harg.options[key] = "";
                if (!expected_empty.at(key))
                {
                    if (harg.pos == harg.argc)
                        return false;
                    harg.options[key] = harg.argv[harg.pos + 1];
                    harg.pos += 2;
                    return true;
                }
            }
        }
        catch (out_of_range)
        {
        }
        return false;
    }

    Program_arg parse_args(int argc, char** argv)
    {
        Arg_handle harg{0, argc, argv, {}};
        vector<Program_input> inputs;
        for (harg.pos = 1; harg.pos < argc; ++harg.pos)
        {
            if (!parse_options(harg))
            {
                string item {harg.argv[harg.pos]};
                Program_input input {harg.options, harg.argv[harg.pos]};
                inputs.push_back(input);
            }
        }
        Program_output output {harg.options.at("output")};
        Program_arg arg {inputs, output};
        return arg;
    }
}

int main(int argc, char** argv)
{
    using std::cout;
    fauces::Program_arg arg = fauces::parse_args(argc, argv);
    fauces::Supply supply;
    for (auto i = arg.inputs.begin(); i != arg.inputs.end(); ++i)
    {
        cout << "Input: " << i->value << "\n";
        fauces::add_to_supply<fauces::arch::Visy>(supply, *i);
    }
    fauces::Linked_program prog = supply.link<fauces::arch::Visy>();
    fauces::save_program<fauces::arch::Visy>(prog, arg.output);
    cout << "Output: " << arg.output.value << "\n";
    return 0;
}
