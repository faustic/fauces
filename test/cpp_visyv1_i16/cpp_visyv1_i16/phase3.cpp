// phase3.cpp
// Phase 3 of translation
// Intended compatibility: c++17
//
// Created by Alejandro Castro García on 9 February 2021
/*
Licensed under the MIT License.
 
Copyright (c) 2021 Faustic Inferno SL
 
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

#include "phase1.hpp"
#include "phase2.hpp"
#include "phase3.hpp"

#include "translator.hpp"


#include "../../visy1010/visy1010/using_iostream.hpp"
#include "../../visy1010/visy1010/using_containers.hpp"
#include "../../visy1010/visy1010/using_string.hpp"
#include "../../visy1010/visy1010/using_memory.hpp"
#include "../../visy1010/visy1010/using_algorithm.hpp"

namespace fauces
{

static void splice(istream& is, Source_context& context, u32string& line)
{
    auto& src = context.src;
    splice_lines(is, line);
    context.line_start += src.col;
    src.col = 0;
    ++src.lineno;
}

static char32_t code16(istream& is, Source_context& context, u32string& line)
{
    auto& src = context.src;
    u32string text = U"u";
    auto start = context.line_start + src.col;
    if (start + 4 >= line.size())
        throw Syntax_error {"Bad universal character name"};
    for (auto i = 0; i < 4; ++i)
        start += line[start + i];
    src.col += 4;
    return universal(text);
}

static char32_t code32(istream& is, Source_context& context, u32string& line)
{
    auto& src = context.src;
    u32string text = U"U";
    auto start = context.line_start + src.col;
    if (start + 8 >= line.size())
        throw Syntax_error {"Bad universal character name"};
    for (auto i = 0; i < 8; ++i)
        start += line[start + i];
    src.col += 8;
    return universal(text);
}

static char32_t escape(istream& is, Source_context& context, u32string& line)
{
    auto& src = context.src;
    if (src.col == line.size())
    {
        splice(is, context, line);
        return 0;
    }
    else
    {
        char32_t c = line[context.line_start + src.col];
        switch (c)
        {
            case 'u':
                return code16(is, context, line);
            case 'U':
                return code32(is, context, line);
        }
    }
    return U'\\';
}

static void unknown_token(Token& token, const u32string& text)
{
    token.type = Token_type::unknown;
    token.text = plainchar_utf8(text);
}

static Token new_line(istream& is, Source_context& context, u32string& line)
{
    auto& src = context.src;
    line = readline(is);
    context.line_start = 0;
    src.col = 0;
    ++src.lineno;
    Token token {src};
    token.text = "\n";
    token.type = Token_type::white;
    return token;
}

static void continue_token(Token& token, Token_type& expected_type,
                           u32string& text, char32_t code)
{
    text += code;
    switch (expected_type)
    {
        case Token_type::unknown:
            unknown_token(token, text);
            return;
        default:
            break;
    }
}

static Token_type expected_type
                        (istream& is, Source_context& context, u32string& line)
{
    // TODO: detect potential white space, identifier or number
    /* * This function should take a peek at the next character in the line,
     * * then deduce the probable token type. It does not matter if the guess
     * * is wrong, it will be corrected later.
     * * For example L"hello" will be first expected to be an identifier
     * * because it starts with an L, but as soon a the first double quotation
     * * mark is found, the expected token type will be changed to string
     * * literal.
     */
    auto& src = context.src;
    if (context.line_start + src.col >= line.size())
        return Token_type::white;
    return Token_type::unknown;
}

static Token next_token(istream& is, Source_context& context, u32string& line)
{
    auto& src = context.src;
    Token token {src};
    u32string text;
    auto type = expected_type(is, context, line);
    vector<Token_type> expected_types;
    while (token.type == Token_type::incomplete)
    {
        if (context.line_start + src.col >= line.size())
            return new_line(is, context, line);
        char32_t c = line[context.line_start + src.col++];
        switch (c)
        {
            case unicode_beot:
                token.type = Token_type::eof;
                break;
            case '\\':
            {
                char32_t code = escape(is, context, line);
                if (code)
                    continue_token(token, type, text, code);
                break;
            }
            default:
                continue_token(token, type, text, c);
        }
    }
    return token;
}

}

auto fauces::Translator::pretokenize(const string& path) -> list<Token>
{
    list<Token> tokens;
    ifstream is;
    is.exceptions(is.failbit | is.badbit | is.eofbit);
    is.open(path, is.binary);
    u32string line;
    line = readline(is, true);
    Source_context context {path};
    Source_location src {path};
    for (Token token {context.src};;)
    {
        token = next_token(is, context, line);
        tokens.push_back(token);
        if (token.type == Token_type::eof)
        {
            auto& last_token = tokens.back();
            last_token.text = "\n";
            last_token.type = Token_type::white;
            break;
        }
    }
    return tokens;
}
