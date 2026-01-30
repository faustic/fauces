//  arg.hpp
//  Utilities to deal with arguments of the main function
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

//

/*
 # EXPLANATION OF INTENT

 When dealing with arguments of the main function, we tend to always use one
 of a few typical schemes. We intend to provide functions here to do the parsing
 for the schemes we use most.
*/

#ifndef faulib_cli_arg_hpp
#define faulib_cli_arg_hpp

#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <limits>
#include <stdexcept>
#include <iostream>



namespace faulib::cli
{
using std::string;
using std::vector;
using std::unordered_map;
using std::map;
using std::numeric_limits;
using std::logic_error;
using std::out_of_range;
using std::ostream;

enum class Dupkey {inherit, empty, replace, append};
/*
 - empty: no value is expected after the key (only its presence or absence has
   meaning)
 - replace: a value is expected after the key, repeating a key replaces the
   previous value for that key
 - append: a value is expected after the key, repeating a key appends a new
   value to the key's value list
 - inherit: behave as empty, replace or append depending on another Dupkey.
 */

struct Key_syntax
{
    bool needed = false;
    Dupkey dupkey = {Dupkey::inherit};
    vector<string> invalid;
};

struct Cmd_syntax
{
    string command_name = "command";
    string arg_name = "arg";
    int min_values = 0;
    int max_values = numeric_limits<int>::max();
    Dupkey dupkey = {Dupkey::inherit};
    map<string, Key_syntax> keys;
};

inline
ostream& print_args(ostream& os, string arg_name, int first, int last)
{
    if (last >= first)
    {
        os << arg_name;
        if (last > first)
        {
            os << first;
            if (last > first + 1)
                os << " ...";
            os << ' ' << arg_name;
            if (last == numeric_limits<int>::max())
                os << 'N';
            else
                os << last;
        }
    }
    return os;
}

inline
ostream& print_optional_args(ostream& os, string arg_name, int first, int last)
{
    if (last >= first)
    {
        os << " [";
        print_args(os, arg_name, first, last);
        os << ']';
    }
    return os;
}

inline
ostream& print_keys
 (ostream& os, const map<string, Key_syntax>& keys, Dupkey defdupkey)
{
    for (auto i = keys.begin(); i != keys.end(); ++i)
    {
        os << ' ';
        if (!i->second.needed)
            os << '[';
        os << "--" << i->first;
        auto dupkey = i->second.dupkey;
        if (dupkey == Dupkey::inherit)
            dupkey = defdupkey;
        if (dupkey == Dupkey::inherit)
            dupkey = Dupkey::empty;
        if (dupkey != Dupkey::empty)
            os << " <" << i->first << '>';
        if (!i->second.needed)
            os << ']';
    }
    return os;
}

inline
ostream& operator <<(ostream& os, const Cmd_syntax& syntax)
{
    os << "\nUsage: \n\n";
    os << syntax.command_name;
    if (!syntax.keys.empty())
        print_keys(os, syntax.keys, syntax.dupkey);
    int min = syntax.min_values;
    int max = syntax.max_values;
    if (min > max)
    {
        os << "(more than " << min;
        os << " but no more than " << max << " arguments????)";
    }
    else
    {
        if (min > 0)
        {
            os << ' ';
            print_args(os, syntax.arg_name, 1, min);
        }
        if (min < numeric_limits<int>::max())
            print_optional_args(os, syntax.arg_name, min + 1, max);
    }
    os << "\n\n";
    return os;
}

struct Arg_with_options
{
    unordered_map<string, vector<string>> options;
    string arg;
};

struct Options_with_args
{
    unordered_map<string, vector<string>> options;
    vector<string> args;
};

class Syntax_error : public logic_error
{
    int argc;
    char** argv;
    Cmd_syntax syntax;
public:
    Syntax_error(int argc, char** argv, const Cmd_syntax& syntax):
    argc {argc}, argv {argv}, syntax {syntax}, logic_error("Syntax error")
    {}
    friend ostream& operator <<(ostream& os, const Syntax_error& error);
};

inline
ostream& operator <<(ostream& os, const Syntax_error& error)
{
    os << error.syntax;
    return os;
}

class Out_of_range_arg : public logic_error
{
public:
    Out_of_range_arg() : logic_error("Argument not in argument list")
    {}
};

class Max_min_error : public logic_error
{
public:
    Max_min_error() : logic_error("Minimum greater than maximum")
    {}
};

class Args
{
public:
    class Parser
    {
    public:
        friend Args;
        bool operator==(const Parser& b) const
        {
            return pos == b.pos;
        }
        Parser& operator++()
        {
            goto_next_arg();
            return *this;
        }
        Parser operator++(int)
        {
            Parser i {*this};
            goto_next_arg();
            return i;
        }
        Arg_with_options operator*()
        {
            if (pos < 1 || pos >= argc)
                throw Out_of_range_arg();
            return Arg_with_options {options, arg};
        }
    private:
        int pos = 0;
        int narg = 0;
        int argc;
        char** argv;
        const Cmd_syntax& syntax;
        unordered_map<string, vector<string>> options;
        vector<string> args;
        string arg;

        Parser(const Cmd_syntax& syntax, int argc, char** argv,
                 bool is_end = false) :
        syntax {syntax}, argc {argc}, argv {argv}
        {
            if (is_end)
                pos = argc;
        }
        int parse(bool all_args = false)
        {
            int last_arg_pos = argc;
            bool expecting_option_value = false;
            string key;
            for (auto i = pos + 1; i < argc; ++i)
            {
                string value {argv[i]};
                if (expecting_option_value)
                {
                    options[key].push_back(value);
                    expecting_option_value = false;
                }
                else if (value.length() >= 2 && value.substr(0, 2) == "--")
                {
                    key = value.substr(2);
                    expecting_option_value = add_option_key(key);
                }
                else
                {
                    arg = value;
                    last_arg_pos = i;
                    if (++narg > syntax.max_values)
                        throw Syntax_error(argc, argv, syntax);
                    if (all_args)
                        args.push_back(arg);
                    else
                        break;
                }
            }
            if (expecting_option_value)
                throw Syntax_error(argc, argv, syntax);
            if (all_args && narg < syntax.min_values)
                throw Syntax_error(argc, argv, syntax);
            return last_arg_pos;
        }
        void final_check()
        {
            for (auto i = syntax.keys.begin(); i != syntax.keys.end(); ++i)
            {
                if (i->second.needed && !options.contains(i->first))
                    throw Syntax_error(argc, argv, syntax);
            }
            if (narg < syntax.min_values)
                throw Syntax_error(argc, argv, syntax);
        }
        void goto_next_arg()
        {
            pos = parse();
            if (pos == argc)
                final_check();
        }
        bool add_option_key(const string& key)
        {
            bool value_expected = true;
            try
            {
                auto key_syntax = syntax.keys.at(key);
                auto dupkey = key_syntax.dupkey;
                if (dupkey == Dupkey::inherit)
                    dupkey = syntax.dupkey;
                switch (dupkey)
                {
                    case Dupkey::empty:
                    case Dupkey::inherit:
                        value_expected = false;
                        [[fallthrough]];
                    case Dupkey::replace:
                        options[key].clear();
                        break;
                    case Dupkey::append:
                        options.try_emplace(key);
                        break;
                }
                for (auto i = key_syntax.invalid.begin();
                     i != key_syntax.invalid.end(); ++i)
                {
                    string key = *i;
                    if (options.contains(key))
                        options.erase(key);
                }
            }
            catch (out_of_range)
            {
                throw Syntax_error(argc, argv, syntax);
            }
            return value_expected;
        }
    };

    Args(Cmd_syntax& syntax, int argc, char** argv) :
    syntax {syntax}, argc {argc}, argv {argv}
    {
        if (this->syntax.dupkey == Dupkey::inherit)
            this->syntax.dupkey = Dupkey::empty;
        if (argc > 0)
            this->syntax.command_name = argv[0];
        if (syntax.min_values > syntax.max_values)
            throw Max_min_error();
    }

    Parser begin() const
    {
        auto p = Parser(syntax, argc, argv);
        p.goto_next_arg();
        return p;
    }

    Parser end() const
    {
        return Parser(syntax, argc, argv, true);
    }

    Options_with_args all() const
    {
        auto p = Parser(syntax, argc, argv);
        p.parse(true);
        p.final_check();
        return Options_with_args {p.options, p.args};
    }

private:
    int argc;
    char** argv;
    Cmd_syntax syntax;
};

} // faulib::cli

#endif /* faulib_cli_arg_hpp */
