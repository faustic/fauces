// phase3.cpp
// Phase 3 of translation
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

#include "phase1.hpp"
#include "phase2.hpp"
#include "phase3.hpp"

#include "translator.hpp"

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

static void next_line(Source_context& context)
{
    auto& src = context.src;
    context.line = readline(context.is);
    context.line_start = 0;
    src.col = 0;
    ++src.lineno;
}

static Token new_line(Source_context& context)
{
    next_line(context);
    Token token {context.src};
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

static void unget_ch(Source_context& context)
{
    if (context.src.col == 0)
        throw Unget_error();
    --context.src.col;
}

static void unget_ch(Source_context& context, size_t times)
{
    while (times--)
        unget_ch(context);
}

static char32_t get_ch(Source_context& context)
{
    char32_t c = peek_ch(context);
    next_ch(context);
    return c;
}

static bool is_control(char32_t c)
{
    return c < 0x1f || (c >= 0x7f && c <= 0x9f);
}

static bool is_basic(char32_t c)
{
    return (c>= 9 && c <= 0xc) || (c >= 0x20 && c<= 0x23) ||
            (c >= 0x25 && c <= 0x3f) || (c >= 0x41 && c <= 0x5f) ||
            (c >= 0x61 && c <= 0x7e);
}

static bool is_digit(char32_t c)
{
    return (c >= U'0' & c <= U'9');
}

using Parse_token = Token (*)(Source_context& context);

static Token unknown_token(Source_context& context)
{
    Token token {context.src, Token_type::unknown};
    token.text = plainchar_utf8(u32string {get_ch(context)});
    return token;
}

static Token eof(Source_context& context)
{
    return Token(context.src, Token_type::eof);
}

static Token parse_div(Source_context& context)
{
    auto& src = context.src;
    Token token {src, Token_type::pp_op_or_punc};
    token.text = "/";
    next_ch(context);
    if (peek_ch(context) == U'/')
    {
        token.text = " ";
        token.type = Token_type::white;
        src.col = context.line.size() - context.line_start;
    }
    else if (peek_ch(context) == U'*')
    {
        next_ch(context);
        token.text = " ";
        token.type = Token_type::white;
        for (;;)
        {
            auto pos = context.line.find(U"*/");
            if (pos == string::npos)
                next_line(context);
            else
            {
                context.src.col = context.line_start + pos + 2;
                break;
            }
        }
    }
    return token;
}

static inline bool in_range(char32_t c, char32_t first, char32_t last)
{
    return c >= first && c <= last;
}

static bool is_nondigit(char32_t c)
{
    return in_range(c, U'A', U'Z')  || c == U'_' || in_range(c, U'a', U'z');
}

static bool is_universal(char32_t c)
{
    return c == 0xa8 || c == 0xaa || c == 0xad || c == 0xaf ||
    in_range(c, 0xb2, 0xb5) || in_range(c, 0xb7, 0xba) ||
    in_range(c, 0xbc, 0xbe) || in_range(c, 0xc0, 0xd6) ||
    in_range(c, 0xd8, 0xf6) || in_range(c, 0xf8, 0xff) ||
    in_range(c, 0x0100, 0x167f) || in_range(c, 0x1681, 0x180d) ||
    in_range(c, 0x180f, 0x1fff) || in_range(c, 0x200b, 0x200d) ||
    in_range(c, 0x202a, 0x202e) || in_range(c, 0x203f, 0x2040) ||
    c == 0x2054 || in_range(c, 0x2060, 0x206f) ||
    in_range(c, 0x2070, 0x218f) || in_range(c, 0x2460, 0x24ff) ||
    in_range(c, 0x2776, 0x2793) || in_range(c, 0x2c00, 0x2dff) ||
    in_range(c, 0x2e80, 0x2fff) || in_range(c, 0x3004, 0x3007) ||
    in_range(c, 0x3021, 0x302f) || in_range(c, 0x3031, 0xd7ff) ||
    in_range(c, 0xf900, 0xfd3d) || in_range(c, 0xfd40, 0xfdcf) ||
    in_range(c, 0xfdf0, 0xfe44) || in_range(c, 0xfe47, 0xfffd) ||
    in_range(c, 0x10000, 0x1fffd) || in_range(c, 0x20000, 0x2fffd) ||
    in_range(c, 0x30000, 0x3fffd) || in_range(c, 0x40000, 0x4fffd) ||
    in_range(c, 0x50000, 0x5fffd) || in_range(c, 0x60000, 0x6fffd) ||
    in_range(c, 0x70000, 0x7fffd) || in_range(c, 0x80000, 0x8fffd) ||
    in_range(c, 0x90000, 0x9fffd) || in_range(c, 0xa0000, 0xafffd) ||
    in_range(c, 0xb0000, 0xbfffd) || in_range(c, 0xc0000, 0xcfffd) ||
    in_range(c, 0xd0000, 0xdfffd) || in_range(c, 0xe0000, 0xefffd);
}

static bool is_identifier_nondigit(char32_t c)
{
    return is_nondigit(c) || is_universal(c);
}

static bool is_identifier_char(char32_t c)
{
    return is_nondigit(c) || is_digit(c) || is_universal(c);
}

static bool is_identifier_start(char32_t c)
{
    if (is_identifier_char(c) && !in_range(c, U'0', U'9') &&
        !in_range(c, 0x300, 0x36f) && !in_range(c, 0x1dc0, 0x1dff) &&
        !in_range(c, 0x20d0, 0x20ff) && !in_range(c, 0xfe20, 0xfe2f))
            return true;
    return false;
}

static Token parse_identifier(Source_context& context)
{
    auto& src = context.src;
    u32string identifier {get_ch(context)};
    Token token {src, Token_type::identifier};
    for (char32_t c = peek_ch(context); is_identifier_char(c);
         c = peek_ch(context))
    {
        identifier += c;
        next_ch(context);
    }
    token.text = plainchar_utf8(identifier);
    return token;
}

static Token parse_white(Source_context& context)
{
    auto& src = context.src;
    Token token{src, Token_type::white};
    token.text = plainchar_utf8(peek_ch(context));
    next_ch(context);
    return token;
}

static Token parse_punc(Source_context &context)
{
    auto& src = context.src;
    Token token{src, Token_type::pp_op_or_punc};
    token.text = plainchar_utf8(peek_ch(context));
    next_ch(context);
    return token;
}

static Token& parse_number(Source_context &context, Token& token)
{
    do
    {
        char32_t c = get_ch(context);
        if (is_digit(c) || is_identifier_nondigit(c) || c == U'.')
            token.text += plainchar_utf8(c);
        else if (c == U'\'')
        {
            char32_t c2 = get_ch(context);
            if (is_digit(c2) || is_nondigit(c2))
                token.text += plainchar_utf8(u32string() + c + c2);
            else
            {
                unget_ch(context, 2);
                break;
            }
        }
        else if (c == U'e' || c == U'E' || c == U'p' || c == U'P')
        {
            char32_t c2 = get_ch(context);
            if (c2 == U'+' || c2 == U'-')
                token.text += plainchar_utf8(u32string() + c + c2);
            else
            {
                unget_ch(context, 2);
                break;
            }
        }
        else
        {
            unget_ch(context);
            break;
        }
    } while (true);
    return token;
}

static Token parse_fullstop(Source_context &context)
{
    auto& src = context.src;
    Token token{src, Token_type::pp_op_or_punc};
    token.text = ".";
    char32_t c = get_ch(context);
    if (is_digit(c))
        return parse_number(context, token);
    else
        return token;
}

static Token parse_number(Source_context &context)
{
    auto& src = context.src;
    Token token{src, Token_type::pp_number};
    token.text = plainchar_utf8(peek_ch(context));
    next_ch(context);
    return parse_number(context, token);
}


unordered_map<char32_t, Parse_token> parse
{
    {U'/', parse_div}, {0x20, parse_white}, {0x09, parse_white},
    {0x0b, parse_white}, {0xff, parse_white}, {U'(', parse_punc},
    {U')', parse_punc}, {U'[', parse_punc}, {U']', parse_punc},
    {U'{', parse_punc}, {U'}', parse_punc}, {U';', parse_punc},
    {U'.', parse_fullstop}, {U'0', parse_number}, {U'1', parse_number},
    {U'2', parse_number}, {U'3', parse_number}, {U'4', parse_number},
    {U'5', parse_number}, {U'6', parse_number}, {U'7', parse_number},
    {U'8', parse_number}, {U'9', parse_number}
};

static Token parse_token(char32_t start, Source_context& context)
{
    auto parser = parse.find(start);
    if (parser != parse.end())
        return parser->second(context);
    else if (is_identifier_start(start))
        return parse_identifier(context);
    return unknown_token(context);
}

static Token next_token(Source_context& context)
{
    char32_t c = peek_ch(context);
    if (c == 0x0a)
        return new_line(context);
    switch (c)
    {
        case unicode_beot:
            return eof(context);
        case '\\':
            next_ch(context);
            c = escape(context);
            if (is_control(c) || is_basic(c))
                throw Invalid_universal();
            [[fallthrough]];
        default:
            return parse_token(c, context);
    }
}

} // namespace fauces

auto fauces::Preprocessor::pretokenize(const string& path) -> list<Token>
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
