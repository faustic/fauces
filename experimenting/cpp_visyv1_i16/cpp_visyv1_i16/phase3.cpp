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

static void splice(Source_context& context)
{
    auto& src = context.src;
    splice_lines(context.is, context.line);
    context.line_start += src.col;
    src.col = 0;
    ++src.lineno;
}

static char32_t code16(Source_context& context)
{
    auto& src = context.src;
    auto& line = context.line;
    u32string text = U"u";
    auto start = context.line_start + src.col;
    if (start + 4 >= line.size())
        throw Syntax_error {"Bad universal character name"};
    for (auto i = 0; i < 4; ++i)
        start += line[start + i];
    src.col += 4;
    return universal(text);
}

static char32_t code32(Source_context& context)
{
    auto& src = context.src;
    auto& line = context.line;
    u32string text = U"U";
    auto start = context.line_start + src.col;
    if (start + 8 >= line.size())
        throw Syntax_error {"Bad universal character name"};
    for (auto i = 0; i < 8; ++i)
        start += line[start + i];
    src.col += 8;
    return universal(text);
}

static char32_t escape(Source_context& context)
{
    auto& src = context.src;
    if (src.col == context.line.size())
    {
        splice(context);
        return 0;
    }
    else
    {
        char32_t c = context.line[context.line_start + src.col];
        switch (c)
        {
            case 'u':
                return code16(context);
            case 'U':
                return code32(context);
        }
    }
    return U'\\';
}

static void unknown_token(Token& token, const u32string& text)
{
    token.type = Token_type::unknown;
    token.text = plainchar_utf8(text);
}

static Token new_line(Source_context& context)
{
    auto& src = context.src;
    context.line = readline(context.is);
    context.line_start = 0;
    src.col = 0;
    ++src.lineno;
    Token token {src};
    token.text = "\n";
    token.type = Token_type::white;
    return token;
}

static char32_t peek_ch(Source_context& context)
{
    auto& src = context.src;
    auto& line = context.line;
    if (context.line_start + src.col >= line.size())
        return 0x0a;
    return line[context.line_start + src.col];
}

static void next_ch(Source_context& context)
{
    ++context.src.col;
}

static Token next_token(Source_context& context)
{
    auto& src = context.src;
    Token token {src};
    char32_t c = peek_ch(context);
    if (c == 0x0a)
        return new_line(context);
    next_ch(context);
    switch (c)
    {
        case unicode_beot:
            token.type = Token_type::eof;
            break;
        case '\\':
            c = escape(context);
            [[fallthrough]];
        default:
        {
            u32string text {c};
            unknown_token(token, text);
        }
    }
    return token;
}

} // namespace fauces

auto fauces::Translator::pretokenize(const string& path) -> list<Token>
{
    list<Token> tokens;
    Source_context context {path};
    context.line = readline(context.is, true);
    for (Token token {context.src};;)
    {
        token = next_token(context);
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
