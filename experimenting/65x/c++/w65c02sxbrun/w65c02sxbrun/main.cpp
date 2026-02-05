//  main.cpp
//  Upload a program to a W65C02SXB board, run the program and get the result
//
//  Created by Alejandro Castro García on 1 September 2021.

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

#include <iostream>

#include "exec.hpp"
#include "sxbenv.hpp"
#include <faulib/io/rawfile.hpp>
#include <faulib/cli/arg.hpp>

static faulib::cli::Options_with_args parse_args(int argc, char** argv)
{
    using faulib::cli::Cmd_syntax;
    using faulib::cli::Dupkey;
    using faulib::cli::Args;
    Cmd_syntax syntax =
    {
        .max_values = 0,
        .dupkey = Dupkey::replace,
        .keys =
        {
            {"input", {.needed = true}},
            {"output", {.needed = true}},
            {"port", {.needed = true}}
        }
    };
    Args args {syntax, argc, argv};
    return args.all();
}

int main(int argc, char ** argv)
{
    using std::cerr;
    using std::string;
    using w65c02::span;
    using w65c02::Executable;
    using w65c02::vector;
    using w65c02::Sxb;
    using w65c02::Byte;
    using w65c02::Address16;
    try
    {
        auto opt = parse_args(argc, argv);
        Executable exec {opt.options.at("input").at(0).c_str()};
        span<Byte> bytes = exec.get_bytes();
        Sxb sxb {opt.options.at("port").at(0)};
        vector<Address16> old_vectors = get_vectors(sxb);
        sxb.from_pc(exec.code_start(), bytes.subspan(exec.code_pos,
                                                     exec.code_size()));
        sxb.run(bridge(sxb, exec.code_start()));
        send_vectors(sxb, old_vectors);
        if (exec.result_size() > 0)
        {
            auto result = sxb.to_pc(exec.result_start(), exec.result_size());
            cerr << "Returned " << result.size() << "-byte result\n";
            const string ofile = opt.options.at("output").at(0);
            faulib::io::writeraw<Byte>(ofile.c_str(), result);
        }
        else
            cerr << "Returned with empty result\n";
    }
    catch (const faulib::cli::Syntax_error& error)
    {
        cerr << error;
        return EXIT_FAILURE;
    }
    return 0;
}
