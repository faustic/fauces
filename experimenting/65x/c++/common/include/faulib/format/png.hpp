//  PNG image reader
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

#ifndef faulib_format_png_hpp
#define faulib_format_png_hpp

#include <array>
#include <vector>
#include <istream>
#include <stdexcept>
#include <cstdint>
#include <cstdlib>
#include <limits>

#include <iostream>
#include <iomanip>


namespace faulib::format
{

using std::uint_least16_t;
using std::uint_least32_t;
using std::uint_least64_t;

class Deflated_err_eof: public std::runtime_error
{
public:
    Deflated_err_eof(const std::string& what_arg): std::runtime_error(what_arg)
    {
    }
};

class Deflated_err_bad: public std::runtime_error
{
public:
    Deflated_err_bad(const std::string& what_arg): std::runtime_error(what_arg)
    {
    }
};

enum Deflated_compression_type
{
    deflated_nocompression,
    deflated_huff_fixed,
    deflated_huff_dyn,
    deflated_reserved
};

class Deflated_decoder
{
public:
    Deflated_decoder(std::vector<unsigned char>::const_iterator begin,
                    std::vector<unsigned char>::const_iterator end):
    begin {begin},
    p {begin},
    end {end}
    {
    }
    
    std::vector<unsigned char> decode_blocks()
    {
        std::vector<unsigned char> output;
        p = begin;
        bitpos = 0;
        finished = false;
        while (!finished)
            decode_block(output);
        uint_least32_t saved_adler = 0;
        for (auto i = 0; i != 4; ++i)
            saved_adler = (saved_adler << 8) | next_byte();
        uint_least16_t s1 = 1;
        uint_least16_t s2 = 0;
        for (auto p = output.begin(); p != output.end(); ++p)
        {
            s1 = (s1 + *p) % 65521;
            s2 = (s2 + s1) % 65521;
        }
        uint_least32_t calculated_adler = ((uint_least32_t)s2 << 16) | s1;
        if (calculated_adler != saved_adler)
            throw Deflated_err_bad("Bad Adler-32 checksum");
        return output;
    }

private:
    const std::vector<unsigned char>::const_iterator begin;
    const std::vector<unsigned char>::const_iterator end;
    std::vector<unsigned char>::const_iterator p;
    int bitpos = 0;
    bool finished = false;
    
    struct Tree_node
    {
        uint_least32_t len;
        uint_least32_t code;
    };
    
    struct Tree
    {
        std::vector<Tree_node> nodes;
        std::vector<uint_least32_t> maxcode;
        uint_least32_t minlen = std::numeric_limits<uint_least32_t>::max();
        uint_least32_t maxlen = 0;
    };
    
    struct Tree_ld
    {
        Tree len_tree;
        std::vector<uint_least16_t> ltranscodes;
        Tree dist_tree;
        std::vector<uint_least16_t> dtranscodes;
    };
    
    void decode_block(std::vector<unsigned char>& output)
    {
        auto bfinal = next_bit();
        if (bfinal)
            finished = true;
        auto btype = next_bits_lsbf(2);
        std::cout << "bfinal: " << (int)bfinal << "\n";
        std::cout << "btype: " << btype << "\n";
        switch (btype)
        {
            case deflated_nocompression:
                decode_nocompressed(output);
                break;
            case deflated_huff_dyn:
                decode_huff_dyn(output);
                break;
            case deflated_huff_fixed:
                decode_huff_fixed(output);
                break;
            case deflated_reserved:
            default:
                throw Deflated_err_bad("Unknown compression type");
                break;
        }
    }
    
    void decode_nocompressed(std::vector<unsigned char>& output)
    {
        if (bitpos)
        {
            bitpos = 0;
            ++p;
        }
        uint_least16_t len = next_uint16();
        uint_least16_t enlen = ~len;
        uint_least16_t nlen = next_uint16();
        std::cout << "len: " <<  len  << "\n";
        if (nlen != enlen)
            throw Deflated_err_bad("Inconsistent block length");
        for (uint_least32_t i = 0; i != len; ++i)
            output.push_back(next_byte());
    }
    
    void decode_huff_dyn(std::vector<unsigned char>& output)
    {
        decode_huff(read_huff_table(), output);
    }
    
    void decode_huff_fixed(std::vector<unsigned char>& output)
    {
        Tree_ld ld;
        for (int i = 0; i <= 143; ++i)
            ld.len_tree.nodes.push_back(Tree_node{8, 0});
        for (int i = 144; i <= 255; ++i)
            ld.len_tree.nodes.push_back(Tree_node{9, 0});
        for (int i = 256; i <= 279; ++i)
            ld.len_tree.nodes.push_back(Tree_node{7, 0});
        for (int i = 280; i <= 287; ++i)
            ld.len_tree.nodes.push_back(Tree_node{8, 0});
        for (int i = 0; i <= 31; ++i)
            ld.dist_tree.nodes.push_back(Tree_node{5, 0});
        calc_codes(ld.len_tree);
        ld.ltranscodes.resize(1 << ld.len_tree.maxlen);
        calc_codes(ld.dist_tree);
        ld.dtranscodes.resize(1 << ld.dist_tree.maxlen);
        for (int j = 0; j != ld.len_tree.nodes.size(); ++j)
        {
            if (ld.len_tree.nodes[j].len)
                ld.ltranscodes[ld.len_tree.nodes[j].code] = j;
        }
        for (int j = 0; j != ld.dist_tree.nodes.size(); ++j)
        {
            if (ld.dist_tree.nodes[j].len)
                ld.dtranscodes[ld.dist_tree.nodes[j].code] = j;
        }
        decode_huff(ld, output);
    }
    
    Tree_ld read_huff_table()
    {
        static constexpr uint_least32_t lengths[] =
            {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
        uint_least32_t hlit = next_bits_lsbf(5);
        uint_least32_t hdist = next_bits_lsbf(5);
        uint_least32_t hclen = next_bits_lsbf(4);
        uint_least32_t n = hclen + 4;
        Tree lctree;
        lctree.nodes.resize(19);
        for (int i = 0; i < n; ++i)
        {
            int j = lengths[i];
            lctree.nodes[j] = {next_bits_lsbf(3), 0};
        }
        for (int i = n; i < 19; ++i)
        {
            int j = lengths[i];
            lctree.nodes[j] = {0, 0};
        }
        std::cout << "hlit: " << hlit << "\n";
        std::cout << "hdist: " << hdist << "\n";
        std::cout << "hclen: " << hclen << "\n";
        calc_codes(lctree);
        std::array<uint_least32_t, 128> transcodes;
        for (int j = 0; j != lctree.nodes.size(); ++j)
        {
            std::cout << "length: " << lctree.nodes[j].len;
            std::cout << ", code: " << lctree.nodes[j].code << "\n";
            if (lctree.nodes[j].len)
                transcodes[lctree.nodes[j].code] = j;
        }
        Tree_ld ld;
        ld.len_tree = read_encoded_tree(lctree, transcodes, hlit + 257, 288);
        ld.ltranscodes.resize(1 << ld.len_tree.maxlen);
        for (int j = 0; j != ld.len_tree.nodes.size(); ++j)
        {
            if (ld.len_tree.nodes[j].len)
                ld.ltranscodes[ld.len_tree.nodes[j].code] = j;
        }
        ld.dist_tree = read_encoded_tree(lctree, transcodes, hdist + 1, 32);
        ld.dtranscodes.resize(1 << ld.dist_tree.maxlen);
        for (int j = 0; j != ld.dist_tree.nodes.size(); ++j)
        {
            if (ld.dist_tree.nodes[j].len)
                ld.dtranscodes[ld.dist_tree.nodes[j].code] = j;
        }
        return ld;
    }
    
    Tree read_encoded_tree(const Tree& tree,
                           const std::array<uint_least32_t, 128>& transcodes,
                           uint_least32_t ncodes, uint_least32_t size)
    {
        Tree newtree;
        uint_least32_t n = ncodes;
        uint_least32_t len = 0;
        while (n)
        {
            auto lencode = read_huff_code(tree);
            std::cout << "lencode: " << lencode << "\n";
            if (lencode > 127)
                    throw Deflated_err_bad("Unexpected length code");
            auto lenkey = transcodes[lencode];
            std::cout << "lenkey: " << lenkey << "\n";
            if (lenkey < 16)
            {
                len = lenkey;
                newtree.nodes.push_back(Tree_node{len, 0});
                --n;
            }
            else if (lenkey == 16)
            {
                uint_least32_t rep = next_bits_lsbf(2) + 3;
                if (rep > n)
                    throw Deflated_err_bad("More length codes than expected");
                for (uint_least32_t i = 0; i < rep; ++i)
                {
                    newtree.nodes.push_back(Tree_node{len, 0});
                    --n;
                }
            }
            else
            {
                uint_least32_t rep;
                if (lenkey == 17)
                    rep = next_bits_lsbf(3) + 3;
                else if (lenkey == 18)
                    rep = next_bits_lsbf(7) + 11;
                else 
                    throw Deflated_err_bad("Unexpected length code");
                if (rep > n)
                    throw Deflated_err_bad("More length codes than expected");
                for (uint_least32_t i = 0; i < rep; ++i)
                {
                    newtree.nodes.push_back(Tree_node{0, 0});
                    --n;
                }
            }
        }
        for (uint_least32_t i = ncodes; i <size; ++i)
            newtree.nodes.push_back(Tree_node{0, 0});
        calc_codes(newtree);
        return newtree;
    }
    
    uint_least32_t read_huff_code(const Tree& tree)
    {
        auto l = tree.minlen;
        uint_least32_t c = next_bits_msbf(l);
        while (l <= tree.maxlen && c > tree.maxcode[l])
        {
            c = (c << 1) | next_bit();
            ++l;
        }
        return c;
    }

    void decode_huff(const Tree_ld& ld, std::vector<unsigned char>& output)
    {
        auto hc = read_huff_code(ld.len_tree);
        uint_least16_t c = ld.ltranscodes.at(hc);
        while (c != 256)
        {
            std::cout << "c: " << c << "\n";
            if (c < 256)
                output.push_back(c);
            else
            {
                uint_least16_t len;
                if (c < 265)
                    len = c - 254;
                else if (c < 285)
                {
                    uint_least16_t nbits = (c - 261) / 4;
                    len = ((c - 261 - 4 * (nbits - 1)) << nbits) + 3 +
                                                        next_bits_lsbf(nbits);
                }
                else if (c == 285)
                    len = 258;
                else
                    throw Deflated_err_bad("Unexpected length code");
                auto hc2 = read_huff_code(ld.dist_tree);
                uint_least16_t c2 = ld.dtranscodes.at(hc2);
                uint_least16_t dist;
                if (c2 < 4)
                    dist = c2 + 1;
                else if (c2 < 30)
                {
                    uint_least16_t nbits = (c2 - 2) / 2;
                    dist = ((c2 - 2 - 2 * (nbits - 1)) << nbits) + 1 +
                                                        next_bits_lsbf(nbits);
                }
                else
                    throw Deflated_err_bad("Unexpected distance code");
                std::cout << "c2: " << c2 << "\n";
                std::cout << "len: " << len << "\n";
                std::cout << "dist: " << dist << "\n";
                output.push_back(output.at(output.size() - dist));
                --len;
                while (len)
                {
                    output.push_back(output[output.size() - dist]);
                    --len;
                }
            }
            hc = read_huff_code(ld.len_tree);
            c = ld.ltranscodes.at(hc);
        }
    }

    static void calc_codes(Tree& tree)
    {
        for (auto p = tree.nodes.begin(); p != tree.nodes.end(); ++p)
        {
            if (p->len > tree.maxlen)
                tree.maxlen = p->len;
        }
        if ((std::size_t)tree.maxlen >
                                std::numeric_limits<std::size_t>::max() - 1)
            throw Deflated_err_bad("Length too big");
        std::vector<uint_least32_t> blcount(tree.maxlen + 1);
        for (auto p = tree.nodes.begin(); p != tree.nodes.end(); ++p)
        {
            if (p->len)
            {
                if (p->len < tree.minlen)
                    tree.minlen = p->len;
                ++blcount[p->len];
            }
        }
        std::vector<uint_least32_t> nextcode(tree.maxlen + 1);
        for (uint_least32_t l = 1; l <= tree.maxlen; ++l)
            nextcode[l] = (nextcode[l - 1] + blcount[l - 1]) << 1;
        tree.maxcode.clear();
        tree.maxcode.resize(tree.maxlen + 1);
        for (auto p = tree.nodes.begin(); p != tree.nodes.end(); ++p)
        {
            if (p->len)
            {
                p->code = nextcode[p->len];
                tree.maxcode[p->len] = p->code;
                ++nextcode[p->len];
            }
        }
    }
    
    unsigned char next_bit()
    {
        if (!bitpos)
            start_byte();
        unsigned char b = (*p >> bitpos) & 1;
        bitpos = (bitpos + 1) & 7;
        if (!bitpos)
            byte_complete();
        return b;
    }
    
    void start_byte()
    {
        if (p == end)
            throw Deflated_err_eof("Premature end of data");
    }
    
    void byte_complete()
    {
        ++p;
    }

    uint_least32_t next_bits_lsbf(int n) // n <= 32
    {
        uint_least32_t value = 0;
        for (int i = 0; i < n; ++i)
            value |= (uint_least32_t)next_bit() << i;
        return value;
    }
    
    uint_least32_t next_bits_msbf(int n) // n <= 32
    {
        uint_least32_t value = 0;
        for (int i = 0; i < n; ++i)
            value = (value << 1) | next_bit();
        return value;
    }
    
    unsigned char next_byte()
    {
        if (bitpos)
        {
            bitpos = 0;
            byte_complete();
        }
        start_byte();
        unsigned char n = *p;
        byte_complete();
        return n;
    } 
    
    uint_least16_t next_uint16()
    {
        if (bitpos)
        {
            bitpos = 0;
            byte_complete();
        }
        start_byte();
        uint_least16_t n = *p;
        byte_complete();
        start_byte();
        n |= (uint_least16_t)(*p) << 8;
        ++p;
        return n;
    } 
};


class Png_err_unimplemented: public std::runtime_error
{
public:
    Png_err_unimplemented(const std::string& what_arg):
    std::runtime_error(what_arg)
    {
    }
};

class Png_err_signat: public std::runtime_error
{
public:
    Png_err_signat(const std::string& what_arg): std::runtime_error(what_arg)
    {
    }
};

class Png_err_crc: public std::runtime_error
{
public:
    Png_err_crc(const std::string& what_arg): std::runtime_error(what_arg)
    {
    }
};

class Png_err_header: public std::runtime_error
{
public:
    Png_err_header(const std::string& what_arg): std::runtime_error(what_arg)
    {
    }
};

class Png_err_type: public std::runtime_error
{
public:
    Png_err_type(const std::string& what_arg): std::runtime_error(what_arg)
    {
    }
};

class Png_err_palette: public std::runtime_error
{
public:
    Png_err_palette(const std::string& what_arg): std::runtime_error(what_arg)
    {
    }
};

class Png_err_data: public std::runtime_error
{
public:
    Png_err_data(const std::string& what_arg): std::runtime_error(what_arg)
    {
    }
};

class Png_err_output: public std::runtime_error
{
public:
    Png_err_output(const std::string& what_arg): std::runtime_error(what_arg)
    {
    }
};

class Png_err_inflate: public std::runtime_error
{
public:
    Png_err_inflate(const std::string& what_arg): std::runtime_error(what_arg)
    {
    }
};

class Png_err_image: public std::runtime_error
{
public:
    Png_err_image(const std::string& what_arg): std::runtime_error(what_arg)
    {
    }
};

class Png_err_filter: public std::runtime_error
{
public:
    Png_err_filter(const std::string& what_arg): std::runtime_error(what_arg)
    {
    }
};

enum Png_colortype
{
    png_gray = 0,
    png_rgb = 2,
    png_indexed  = 3,
    png_gray_alpha = 4,
    png_rgba = 6
};

enum Png_filtertype
{
    png_filternone = 0,
    png_filtersub = 1,
    png_filterup = 2,
    png_filteraverage = 3,
    png_filterpaeth = 4,
};

class Png
{
public:
    using Uint32 = std::uint_least32_t;
    Png(std::istream& stream)
    {
        unsigned char c[8];
        stream.read(reinterpret_cast<char*>(&c), 8);
        for (int i = 0; i < sizeof(signature); ++i)
        {
            if (c[i] != signature[i])
                throw Png_err_signat("Bad PNG signature");
        }
        while (read_chunk(stream))
        {
        }
    }
    
    const std::vector<unsigned char>& rgba16_pixels()
    {
        if (dst_bitdepth != 16)
            throw(Png_err_output("Unsupported internal representation"));
        return dst_pixels;
    }
    
    std::vector<unsigned char> rgb8_pixels()
    {
        if (dst_bitdepth != 16)
            throw(Png_err_output("Unsupported internal representation"));
        std::vector<unsigned char> rgb8;
        for (size_t i = 0; i != dst_pixels.size(); i += 2)
        {
            if (i % 8 == 6)
                continue;
            rgb8.push_back(dst_pixels[i]);
        }
        return rgb8;
    }
    
    uint_least32_t width()
    {
        return im_width;
    }
    
    uint_least32_t height()
    {
        return im_height;
    }

private:

    // Destination data
    std::vector<unsigned char> dst_pixels;
    unsigned char dst_colortype = png_rgba; // Default for destination is RGBA
    unsigned char dst_bitdepth = 16; // Default for destination is 16 bps.
    
    // Source data
    std::vector<unsigned char> palette;
    std::vector<unsigned char> deflated;
    uint_least32_t bpp = 1; // Bytes per complete pixel rounding up to one 
    unsigned char src_colortype = 0xff; // Initially invalid
    unsigned char src_bitdepth = 0xff; // Initially invalid
    uint_least32_t im_width = 0;
    uint_least32_t im_height = 0;
    
    // Temporary and intermediate results    
    std::vector<unsigned char> prevline;
    std::vector<unsigned char> curline;
    uint_least32_t curbytepos = 0; 
    using Filter = unsigned char (Png::*)(unsigned char);
    Filter filter = &Png::unfilter_none;
    using Convert = void (Png::*)(unsigned char);
    Convert convert = &Png::convert_rgba16_torgba16;
    

    // Definitions
    struct Header
    {
        uint_least32_t width;
        uint_least32_t height;
        unsigned char bitdepth;
        unsigned char colortype;
        unsigned char compress;
        unsigned char filter;
        unsigned char interlace;
    };
    
    static constexpr
    unsigned char signature[] = {137, 80, 78, 71, 13, 10, 26, 10};    

    bool read_chunk(std::istream& stream)
    {
        uint_least32_t length = read_uint32(stream);
        uint_least32_t type = read_uint32(stream);
        std::vector<unsigned char> d(length);
        if (length > 0)
            stream.read(reinterpret_cast<char*>(d.data()), length);
        uint_least32_t crc = read_uint32(stream);
        uint_least32_t ecrc = expected_crc(type, d);
        if (crc != ecrc)
            throw Png_err_crc("Bad PNG CRC");
        bool header_ok {false};
        switch (type)
        {
            case 0x49484452: // IHDR
                prepare_decoding(read_header(d));
                header_ok = true;
                break;
            case 0x504c5445: // PLTE
                read_palette(d);
                break;
            case 0x49444154: // IDAT
                append_pixels(d);
                break;
            case 0x49454e44: // IEND
                read_pixels(deflated);
                return false;
            default:
                std::cout << "type: 0x" << std::hex << type << std::dec;
                std:: cout << " (";
                for (int i = 0; i < 4; ++i)
                {
                    unsigned char c = (unsigned char)(type >> (24 - 8 * i));
                    std::cout << c;
                }
                std::cout << ")\n";
                if (!(type & (1UL << 29)))
                    throw Png_err_type("Unknown critical PNG chunk type");
                break;
                
        }
        return true;
    }
    
    static uint_least32_t read_uint32(const unsigned char* c)
    {
        return ((uint_least32_t)c[0] << 24) | ((uint_least32_t)c[1] << 16) |
               ((uint_least32_t)c[2] << 8) | c[3];
    }
    
    static uint_least32_t read_uint32(std::istream& stream)
    {
        unsigned char c[4];
        stream.read(reinterpret_cast<char*>(&c), 4);
        return read_uint32(c);
    }
    
    static uint_least32_t expected_crc
        (uint_least32_t chunk_type, const std::vector<unsigned char>& data)
    {
        static std::vector<uint_least32_t> crc_table = init_crc_table();
        uint_least32_t c = 0xffff'ffff;
        for (uint_least32_t n = 0; n < 4; ++n)
        {
            unsigned char ch = chunk_type >> (24 - 8 * n);
            uint_least32_t i = (c ^ ch) & 0xff;
            c =  crc_table[i] ^ (c >> 8);
        }
        for (uint_least32_t n = 0; n < data.size(); ++n)
        {
            uint_least32_t i = (c ^ data[n]) & 0xff;
            c =  crc_table[i] ^ (c >> 8);
        }
        return (~c) & 0xffff'ffff;
    }
    
    static std::vector<uint_least32_t> init_crc_table()
    {
        std::vector<uint_least32_t> v;
        for (uint_least32_t n = 0; n < 256; ++n)
        {
            uint_least32_t c = n;
            for (int k = 0; k < 8; ++k)
            {
                if (c & 1)
                    c = 0xedb88320 ^ (c >> 1);
                else
                    c >>= 1;
            }
            v.push_back(c);
        }
        return v;
    };
    
    static Header read_header(const std::vector<unsigned char>& data)
    {
        unsigned char interlace = data.at(12);
        Header header
        {
            read_uint32(&data[0]),
            read_uint32(&data[4]),
            data[8],
            data[9],
            data[10],
            data[11],
            interlace
        };
        switch (header.colortype)
        {
            case png_gray:
            case png_rgb:
            case png_indexed:
            case png_gray_alpha:
            case png_rgba:
                break;
            default:
                throw Png_err_header("Unknown PNG color type");
        }
        if (header.compress != 0)
            throw Png_err_header("Unknown PNG compression method");
        if (header.filter != 0)
            throw Png_err_header("Unknown PNG filter method");
        if (header.interlace != 0 && header.interlace != 1)
            throw Png_err_header("Unknown PNG interlace method");
        return header;
    }
    
    void prepare_decoding(const Header& header)
    {
        std::cout << "width: " << header.width << "\n";
        std::cout << "height: " << header.height << "\n";
        std::cout << "bitdepth: " << (int)header.bitdepth << "\n";
        std::cout << "colortype: " << (int)header.colortype << "\n";
        std::cout << "compress: " << (int)header.compress << "\n";
        std::cout << "filter: " << (int)header.filter << "\n";
        std::cout << "interlace: " << (int)header.interlace << "\n";
        src_colortype = header.colortype;
        src_bitdepth = header.bitdepth;
        im_width = header.width;
        im_height = header.height;
        if (im_width >= 0x8000'0000UL || im_height >= 0x8000'0000UL)
            throw Png_err_header("Image too big");
        uint_least64_t size = (uint_least64_t)im_width * im_height;
        if (size > std::numeric_limits<std::size_t>::max())
            throw Png_err_header("Image takes too much memory");
        
        switch (dst_colortype)
        {
            case png_rgba:
            {
                switch (dst_bitdepth)
                {
                    case 16:
                        break;
                    default:
                        throw(Png_err_output("Unsupported output format"));
                }
                break;
            }
            default:
                throw(Png_err_output("Unsupported output format"));
        }
        if (header.interlace)
            throw Png_err_unimplemented("Interlace pending");
        uint_least32_t spp = 1;
        switch (src_colortype)
        {
            case png_gray:
                bpp = 1;
                switch (src_bitdepth)
                {
                    case 1:
                        convert = &Png::convert_gray1_torgba16;
                        break;
                    case 2:
                        convert = &Png::convert_gray2_torgba16;
                        break;
                    case 4:
                        convert = &Png::convert_gray4_torgba16;
                        break;
                    case 8:
                        convert = &Png::convert_gray8_torgba16;
                        break;
                    case 16:
                        bpp = 2;
                        convert = &Png::convert_gray16_torgba16;
                        break;
                    default:
                        throw Png_err_image("Invalid bit depth");
                }
                break;
            case png_rgb:
                spp = 3;
                if (src_bitdepth == 8)
                {
                    bpp = 3;
                    convert = &Png::convert_rgb8_torgba16;
                }
                else if (src_bitdepth == 16)
                {
                    bpp = 6;
                    convert = &Png::convert_rgb16_torgba16;
                }   
                else
                    throw Png_err_image("Invalid bit depth");
                break;
            case png_indexed:
                bpp = 1;
                switch (src_bitdepth)
                {
                    case 1:
                        convert = &Png::convert_indexed1_torgba16;
                        break;
                    case 2:
                        convert = &Png::convert_indexed2_torgba16;
                        break;
                    case 4:
                        convert = &Png::convert_indexed4_torgba16;
                        break;
                    case 8:
                        convert = &Png::convert_indexed8_torgba16;
                        break;
                    default:
                        throw Png_err_image("Invalid bit depth");
                }
                break;
            case png_gray_alpha:
                spp = 2;
                switch (src_bitdepth)
                {
                    case 8:
                        bpp = 2;
                        convert = &Png:: convert_graya8_torgba16;
                        break;
                    case 16:
                        bpp = 4;
                        convert = &Png:: convert_graya16_torgba16;
                        break;
                    default:
                    throw Png_err_image("Invalid bit depth");
                }
                break;
            case png_rgba:
                spp = 4;
                if (src_bitdepth == 8)
                {
                    bpp = 4;
                    convert = &Png::convert_rgba8_torgba16;
                }
                else if (src_bitdepth == 16)
                {
                    bpp = 8;
                    convert = &Png::convert_rgba16_torgba16;
                }   
                else
                    throw Png_err_image("Invalid bit depth");
                break;
            default:
                throw Png_err_palette("PLTE chunk must be after IHDR chunk");                
        }
        auto linesizebits =
            (uint_least64_t)(im_width + 1) * spp * src_bitdepth;
        uint_least32_t incompletebyte_bits = linesizebits % 8;
        if (incompletebyte_bits)
            linesizebits += 8 - incompletebyte_bits;
        std::cout << "WIDTH: " << im_width << "\n";
        std::cout << "src_bitdepth: " << (int)src_bitdepth << "\n";
        std::cout << "linesizebits: " << linesizebits << "\n";
        uint_least32_t linesize = (uint_least32_t)(linesizebits / 8);
        prevline.resize(linesize);
        curline.resize(linesize);
        std::cout << "linesize: " << linesize << "\n";
    }
    
    void read_palette(const std::vector<unsigned char>& data)
    {
        validate_plte_chunk(data);
        palette.clear();
        for (auto p = data.begin(); p != data.end(); ++p)
            palette.push_back(*p);        
    }
    
    void validate_plte_chunk(const std::vector<unsigned char>& data)
    {
        if (data.size() > 256 * 3)
            throw Png_err_palette("Palette too big");
        if (data.size() < 3)
            throw Png_err_palette("Palette too small");
        if (data.size() % 3)
            throw Png_err_palette("Each palette entry must take three bytes");
        switch (src_colortype)
        {
            case png_rgb:
            case png_indexed:
            case png_rgba:
                break;
            case png_gray:
            case png_gray_alpha:
                throw Png_err_palette("Grayscale images must lack PLTE chunk");
            default:
                throw Png_err_palette("PLTE chunk must be after IHDR chunk");                
        }
    }
    
    void append_pixels(const std::vector<unsigned char>& data)
    {
        deflated.insert(deflated.end(), data.begin(), data.end());
    }

    void read_pixels(const std::vector<unsigned char>& data)
    {
        validate_idat_chunk(data);
        auto infl = inflate(data);
        uint_least32_t lines = 0;
        for (auto p = infl.begin(); p != infl.end(); ++p)
        {
            if (curbytepos == 0)
            {
                std::cout << "filter: " << (int)*p << "\n";
                update_filter(*p);
                ++p;
                curbytepos = bpp;
            }
            curline[curbytepos] = (this->*filter)(*p);
            (this->*convert)(curline[curbytepos]);
            ++curbytepos;
            if (curbytepos == curline.size())
            {
                prevline = curline;
                curbytepos = 0;
                ++lines;
            }
        }
        std::cout << "lines: " << lines << "\n";
        std::cout << "curbytepos: " << curbytepos << "\n";
    }
    
    void validate_idat_chunk(const std::vector<unsigned char>& data)
    {
        switch (src_colortype)
        {
            case png_gray:
            case png_rgb:
            case png_gray_alpha:
            case png_rgba:
                break;
            case png_indexed:
                if (!palette.size())
                    throw Png_err_data("IDAT chunk must be after PLTE chunk");                
                break;
            default:
                throw Png_err_data("IDAT chunk must be after IHDR chunk");                
        }
        std::cout << "data size: " << data.size() << "\n";        
    }

    static std::vector<unsigned char>
    inflate(const std::vector<unsigned char>& src)
    {
        if (src.size() < 7)
            throw Png_err_inflate("Stream too small");        
        std::vector<unsigned char> dst;        
        unsigned char cmf = src[0];
        unsigned char cm = cmf & 0xf;
        unsigned char cinfo = cmf >> 4;
        std::cout << "cm: " << (int)cm << "\n";
        std::cout << "cinfo: " << (int)cinfo << "\n";
        if (cm != 8)
            throw Png_err_inflate("Bad compression method");
        if (cinfo > 7)
            throw Png_err_inflate("Bad window size");
        uint_least16_t wsize = 256 << cinfo;
        std::cout << "window size: " << wsize << "\n";
        unsigned char flg = src[1];
        std::cout << "flg: " << (int)flg << "\n";
        if ((cmf * 256 + flg) % 31)
            throw Png_err_inflate("Bad FCHECK");
        bool fdict = (flg >> 5) & 1;
        std::cout << "fdict: " << (fdict ? "true" : "false") << "\n";
        if (fdict)
            throw Png_err_inflate("Preset dictionary must not be used in PNG");        
        
        Deflated_decoder decoder(src.begin() + 2, src.end());
        return decoder.decode_blocks();
    }
    
    void update_filter(unsigned char b)
    {
        switch (b)
        {
            case png_filternone:
                filter = &Png::unfilter_none;
                break;
            case png_filtersub:
                filter = &Png::unfilter_sub;
                break;
            case png_filterup:
                filter = &Png::unfilter_up;
                break;
            case png_filteraverage:
                filter = &Png::unfilter_average;
                break;
            case png_filterpaeth:
                filter = &Png::unfilter_paeth;
                break;
            default:
                throw Png_err_filter("Invalid filter code");
                break;                
        }
    }
    
    unsigned char unfilter_none(unsigned char unfiltered)
    {
        return unfiltered;
    }
     
    unsigned char unfilter_sub(unsigned char sub)
    {
        return (sub + curline[curbytepos - bpp]) & 0xff;
    }
    
    unsigned char unfilter_up(unsigned char sub)
    {
        return (sub + prevline[curbytepos]) & 0xff;
    }
    
    unsigned char unfilter_average(unsigned char av)
    {
        int a = curline[curbytepos - bpp];
        int b = prevline[curbytepos];
        return (av + (a + b) / 2);
    };
    
    unsigned char unfilter_paeth(unsigned char paeth)
    {
        int a = curline[curbytepos - bpp];
        int b = prevline[curbytepos];
        int c = prevline[curbytepos - bpp];
        return (paeth + paethpredictor(a, b, c)) & 0xff;
    }
    
    unsigned paethpredictor(unsigned char a, unsigned char b, unsigned char c)
    {
        int p = a + b - c;
        int pa = std::abs(p - a);
        int pb = std::abs(p - b);
        int pc = std::abs(p - c);
        if (pa <= pb && pa <= pc)
            return a;
        else if (pb <= pc)
            return b;
        else
            return c;
    }
    
    void convert_gray1_torgba16(unsigned char b)
    {
        int n = 8;
        if (curbytepos == curline.size() - 1)
        {
            int r = im_width % 8;
            if (r)
                n = r;
        }
        for (int i = n - 1; i >= 0; --i)
        {
            unsigned char val = ((b >> i) & 1) ? 0xff : 0x00;
            for (int j = 0; j < 6; ++j)
                dst_pixels.push_back(val);

            // Add alpha for opaque image
            dst_pixels.push_back(0xff);
            dst_pixels.push_back(0xff);
        }
    }
    
    void convert_gray2_torgba16(unsigned char b)
    {
        int n = 4;
        if (curbytepos == curline.size() - 1)
        {
            int r = im_width % 4;
            if (r)
                n = r;
        }
        for (int i = n - 1; i >= 0; --i)
        {
            unsigned val = ((b >> (i * 2)) & 3) * 21845;
            for (int j = 0; j < 3; ++j)
            {
                dst_pixels.push_back(val >> 8);
                dst_pixels.push_back(val & 0xff);
            }

            // Add alpha for opaque image
            dst_pixels.push_back(0xff);
            dst_pixels.push_back(0xff);
        }
    }
    
    void convert_gray4_torgba16(unsigned char b)
    {
        int n = 2;
        if (curbytepos == curline.size() - 1)
        {
            int r = im_width % 2;
            if (r)
                n = r;
        }
        for (int i = n - 1; i >= 0; --i)
        {
            unsigned val = ((b >> (i * 4)) & 0xf) * 4369;
            for (int j = 0; j < 3; ++j)
            {
                dst_pixels.push_back(val >> 8);
                dst_pixels.push_back(val & 0xff);
            }

            // Add alpha for opaque image
            dst_pixels.push_back(0xff);
            dst_pixels.push_back(0xff);
        }
    }
    
    void convert_gray8_torgba16(unsigned char b)
    {
            unsigned val = b * 257;
            for (int j = 0; j < 3; ++j)
            {
                dst_pixels.push_back(val >> 8);
                dst_pixels.push_back(val & 0xff);
            }

            // Add alpha for opaque image
            dst_pixels.push_back(0xff);
            dst_pixels.push_back(0xff);
    }
    
    void convert_gray16_torgba16(unsigned char b)
    {
        if (curbytepos % 2)
        {
            unsigned char b1 = dst_pixels.back();
            dst_pixels.push_back(b);                
            dst_pixels.push_back(b1);
            dst_pixels.push_back(b);                
            dst_pixels.push_back(b1);
            dst_pixels.push_back(b);                

            // Add alpha for opaque image
            dst_pixels.push_back(0xff);
            dst_pixels.push_back(0xff);                
        }
        else
            dst_pixels.push_back(b);
    }
    
    void convert_rgb8_torgba16(unsigned char b)
    {
        dst_pixels.push_back(b);
        dst_pixels.push_back(b > 0x80 ? 0xff : 0x00);
        constexpr unsigned src_bpp = 3;
        if (curbytepos % src_bpp == src_bpp - 1)
        {
            // Add alpha for opaque image
            dst_pixels.push_back(0xff);
            dst_pixels.push_back(0xff);
        }
    }
    
    void convert_rgb16_torgba16(unsigned char b)
    {
        dst_pixels.push_back(b);
        constexpr unsigned src_bpp = 6;
        if (curbytepos % src_bpp == src_bpp - 1)
        {
            // Add alpha for opaque image
            dst_pixels.push_back(0xff);
            dst_pixels.push_back(0xff);
        }
    }
    
    void convert_indexed1_torgba16(unsigned char b)
    {
        int n = 8;
        if (curbytepos == curline.size() - 1)
        {
            int r = im_width % 8;
            if (r)
                n = r;
        }
        for (int i = n - 1; i >= 0; --i)
        {
            unsigned index = ((b >> i) & 1) * 3;
            unsigned blue = palette.at(index + 2) * 257;
            unsigned green = palette[index + 1] * 257;
            unsigned red = palette[index] * 257;
            dst_pixels.push_back (red >> 8);
            dst_pixels.push_back (red & 0xff);
            dst_pixels.push_back (green >> 8);
            dst_pixels.push_back (green & 0xff);
            dst_pixels.push_back (blue >> 8);
            dst_pixels.push_back (blue & 0xff);

            // Add alpha for opaque image
            dst_pixels.push_back(0xff);
            dst_pixels.push_back(0xff);
        }
    }
    
    void convert_indexed2_torgba16(unsigned char b)
    {
        int n = 4;
        if (curbytepos == curline.size() - 1)
        {
            int r = im_width % 4;
            if (r)
                n = r;
        }
        for (int i = n - 1; i >= 0; --i)
        {
            unsigned index = ((b >> (i * 2)) & 3) * 3;
            unsigned blue = palette.at(index + 2) * 257;
            unsigned green = palette[index + 1] * 257;
            unsigned red = palette[index] * 257;
            dst_pixels.push_back (red >> 8);
            dst_pixels.push_back (red & 0xff);
            dst_pixels.push_back (green >> 8);
            dst_pixels.push_back (green & 0xff);
            dst_pixels.push_back (blue >> 8);
            dst_pixels.push_back (blue & 0xff);

            // Add alpha for opaque image
            dst_pixels.push_back(0xff);
            dst_pixels.push_back(0xff);
        }
    }
    
    void convert_indexed4_torgba16(unsigned char b)
    {
        int n = 2;
        if (curbytepos == curline.size() - 1)
        {
            int r = im_width % 2;
            if (r)
                n = r;
        }
        for (int i = n - 1; i >= 0; --i)
        {
            unsigned index = ((b >> (i * 4)) & 0xf) * 3;
            unsigned blue = palette.at(index + 2) * 257;
            unsigned green = palette[index + 1] * 257;
            unsigned red = palette[index] * 257;
            dst_pixels.push_back (red >> 8);
            dst_pixels.push_back (red & 0xff);
            dst_pixels.push_back (green >> 8);
            dst_pixels.push_back (green & 0xff);
            dst_pixels.push_back (blue >> 8);
            dst_pixels.push_back (blue & 0xff);

            // Add alpha for opaque image
            dst_pixels.push_back(0xff);
            dst_pixels.push_back(0xff);
        }
    }
    
    void convert_indexed8_torgba16(unsigned char b)
    {
        unsigned index = b * 3;
        unsigned blue = palette.at(index + 2) * 257;
        unsigned green = palette[index + 1] * 257;
        unsigned red = palette[index] * 257;
        dst_pixels.push_back (red >> 8);
        dst_pixels.push_back (red & 0xff);
        dst_pixels.push_back (green >> 8);
        dst_pixels.push_back (green & 0xff);
        dst_pixels.push_back (blue >> 8);
        dst_pixels.push_back (blue & 0xff);

        // Add alpha for opaque image
        dst_pixels.push_back(0xff);
        dst_pixels.push_back(0xff);
    }
    
    void convert_graya8_torgba16(unsigned char b)
    {
        unsigned val = b * 257; // Grayscale value or alpha, depending on
                                // curbytepos
        unsigned char hval = val >> 8;
        unsigned char lval = val & 0xff;
        dst_pixels.push_back(hval);
        dst_pixels.push_back(lval);
        constexpr unsigned src_bpp = 2;
        if (curbytepos % src_bpp != src_bpp - 1)
        {
            // This was a grayscale value and I just appended
            // the red component. Now append the green  and blue components
            dst_pixels.push_back(hval);
            dst_pixels.push_back(lval);
            dst_pixels.push_back(hval);
            dst_pixels.push_back(lval);
        }
    }
    
    void convert_graya16_torgba16(unsigned char b)
    {
        dst_pixels.push_back(b);    // Part of grayscale value or part of
                                    // alpha, depending on curbytepos
        constexpr unsigned src_bpp = 4;
        if (curbytepos % src_bpp == 1)
        {
            // This was the last part of a grayscale value and I just appended
            // the red component. Now append the green  and blue components
            dst_pixels.push_back(curline[curbytepos - 1]);
            dst_pixels.push_back(b);
            dst_pixels.push_back(curline[curbytepos - 1]);
            dst_pixels.push_back(b);
        }
    }
    
    void convert_rgba8_torgba16(unsigned char b)
    {
        unsigned val = b * 257; // Red, green, blue or alpha, depending on
                                // curbytepos
        unsigned char hval = val >> 8;
        unsigned char lval = val & 0xff;
        dst_pixels.push_back(hval);
        dst_pixels.push_back(lval);
    }
    
    void convert_rgba16_torgba16(unsigned char b)
    {
        dst_pixels.push_back(b);
    }
};

}


#endif // faulib_format_png_hpp
