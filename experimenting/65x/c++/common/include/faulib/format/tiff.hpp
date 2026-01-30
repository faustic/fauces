//  TIFF image format manipulation helper
//
//  Created by Alejandro Castro García on 1 July 2020.
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

#ifndef faulib_format_tiff_hpp
#define faulib_format_tiff_hpp

#include <faulib/format/memory.hpp>

#include <cstdint>
#include <cstddef>
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <span>

namespace faulib::format
{

enum
{
    tiff_white_is_zero = 0,
    tiff_black_is_zero = 1,
    tiff_rgb = 2,
    tiff_palette_color = 3,
    tiff_transparency_mask = 4
};

enum
{
    tiff_byte = 1,
    tiff_ascii = 2,
    tiff_short = 3,
    tiff_long = 4,
    tiff_rational = 5
};

enum
{
    tiff_width_tag = 0x100,
    tiff_length_tag = 0x101,
    tiff_bps_tag = 0x102,
    tiff_compression_tag = 0x103,
    tiff_photometric_tag = 0x106,
    tiff_stripoffsets_tag = 0x111,
    tiff_samplesperpixel_tag = 0x115,
    tiff_rowsperstrip_tag = 0x116,
    tiff_stripbytecounts_tag = 0x117,
    tiff_xres_tag = 0x11a,
    tiff_yres_tag = 0x11b,
    tiff_resunit_tag = 0x128,
    tiff_colormap_tag = 0x140,
    tiff_extrasamples_tag = 0x152,
};

enum
{
    tiff_nocompression = 1,
    tiff_ccitt = 2,
    tiff_packbits = 32773
};

enum
{
    tiff_nounits = 1,
    tiff_inch = 2,
    tiff_centimeter = 3
};

enum
{
    tiff_lastifd = 0
};

class Tiff_be : Memory_be<std::size_t, unsigned char, std::uint_least16_t,
                    std::uint_least32_t, std::uint_least64_t>
{
public:
    Tiff_be(std::size_t datasize = 0): bytes(pixdata_offset + datasize)
    {
        write16(0, 0x4d4d);
        write16(2, 42);
        write32(4, header_size);
        write16(number_of_entries_offset, 0);
        write32(number_of_entries_offset + 2, tiff_lastifd);
    }
    
    unsigned char read8(std::size_t addr) override
    {
        return bytes.at(addr);
    }

    void write8(std::size_t addr, unsigned char value) override
    {
        bytes.at(addr) = value;
    }
    
    void copybytes(unsigned char* dst, std::size_t size) override
    {
        std::copy(bytes.begin(), bytes.end(), dst);
    }

    void resize(std::size_t datasize)
    {
        bytes.resize(pixdata_offset + datasize);
    }

    unsigned char* data()
    {
        return bytes.data();
    }
    
    std::size_t data_filesize()
    {
        return filesize;
    }
    
    unsigned char* pixdata()
    {
        return bytes.data() + pixdata_offset;
    }
    
    std::size_t pixdata_maxsize()
    {
        return bytes.size() - pixdata_offset;
    }
    
    void make_bilevel(std::uint_least16_t width, std::uint_least16_t height)
    {
        auto extrabits = width % 16;
        if (extrabits)
            width += 16 - extrabits;
        std::uint_least32_t bytecount =
                                    (std::uint_least32_t)width * height / 8;
        auto next_entry_offset = first_entry_offset;
        create_short_field(next_entry_offset, tiff_width_tag, width);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_length_tag, height);
        next_entry_offset += entry_size;
        create_short_field( next_entry_offset, tiff_compression_tag,
                            tiff_nocompression);
        next_entry_offset += entry_size;
        create_short_field( next_entry_offset, tiff_photometric_tag,
                            tiff_white_is_zero);
        next_entry_offset += entry_size;
        create_long_field(  next_entry_offset, tiff_stripoffsets_tag,
                            pixdata_offset);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_rowsperstrip_tag, height);
        next_entry_offset += entry_size;
        create_long_field(  next_entry_offset, tiff_stripbytecounts_tag,
                            bytecount);
        next_entry_offset += entry_size;
        create_rational_field(  next_entry_offset, xres_offset, tiff_xres_tag,
                                72, 1);
        next_entry_offset += entry_size;
        create_rational_field(  next_entry_offset, yres_offset, tiff_yres_tag,
                                72, 1);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_resunit_tag, tiff_inch);
        next_entry_offset += entry_size;
        write32(next_entry_offset, tiff_lastifd);
        auto n = (next_entry_offset - first_entry_offset) / entry_size;
        write16(number_of_entries_offset, (std::uint_least16_t)n);
        filesize = pixdata_offset + bytecount;
    }
    
    void make_palette4(std::uint_least16_t width, std::uint_least16_t height,
                       const std::vector<std::uint_least16_t>& colormap)
    {
        std::uint_least32_t bytecount = (std::uint_least32_t)width * height;
        auto next_entry_offset = first_entry_offset;
        create_short_field(next_entry_offset, tiff_width_tag, width);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_length_tag, height);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_bps_tag, 4);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_compression_tag,
                           tiff_nocompression);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_photometric_tag,
                           tiff_palette_color);
        next_entry_offset += entry_size;
        create_long_field(next_entry_offset, tiff_stripoffsets_tag,
                          pixdata_offset);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_rowsperstrip_tag, height);
        next_entry_offset += entry_size;
        create_long_field(next_entry_offset, tiff_stripbytecounts_tag,
                          bytecount);
        next_entry_offset += entry_size;
        create_rational_field(next_entry_offset, xres_offset, tiff_xres_tag,
                              72, 1);
        next_entry_offset += entry_size;
        create_rational_field(next_entry_offset, yres_offset, tiff_yres_tag,72,
                              1);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_resunit_tag, tiff_inch);
        next_entry_offset += entry_size;
        if (colormap.size() <= colormap_size4)
            create_nshort_field(next_entry_offset, colormap_offset,
                                tiff_colormap_tag, colormap);
        else
        {
            auto cm = colormap;
            cm.resize(colormap_size4);
            create_nshort_field(next_entry_offset, colormap_offset,
                                tiff_colormap_tag, cm);
        }
        next_entry_offset += entry_size;
        write32(next_entry_offset, tiff_lastifd);
        auto n = (next_entry_offset - first_entry_offset) / entry_size;
        write16(number_of_entries_offset, (std::uint_least16_t)n);
        filesize = pixdata_offset + bytecount;
    }
    
    void make_palette8(std::uint_least16_t width, std::uint_least16_t height,
                       const std::vector<std::uint_least16_t>& colormap)
    {
        std::uint_least32_t bytecount = (std::uint_least32_t)width * height;
        auto next_entry_offset = first_entry_offset;
        create_short_field(next_entry_offset, tiff_width_tag, width);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_length_tag, height);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_bps_tag, 8);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_compression_tag,
                           tiff_nocompression);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_photometric_tag,
                           tiff_palette_color);
        next_entry_offset += entry_size;
        create_long_field(next_entry_offset, tiff_stripoffsets_tag,
                          pixdata_offset);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_rowsperstrip_tag, height);
        next_entry_offset += entry_size;
        create_long_field(next_entry_offset, tiff_stripbytecounts_tag,
                          bytecount);
        next_entry_offset += entry_size;
        create_rational_field(next_entry_offset, xres_offset, tiff_xres_tag,
                              72, 1);
        next_entry_offset += entry_size;
        create_rational_field(next_entry_offset, yres_offset, tiff_yres_tag,72,
                              1);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_resunit_tag, tiff_inch);
        next_entry_offset += entry_size;
        if (colormap.size() <= colormap_size8)
            create_nshort_field(next_entry_offset, colormap_offset,
                                tiff_colormap_tag, colormap);
        else
        {
            auto cm = colormap;
            cm.resize(colormap_size8);
            create_nshort_field(next_entry_offset, colormap_offset,
                                tiff_colormap_tag, cm);
        }
        next_entry_offset += entry_size;
        write32(next_entry_offset, tiff_lastifd);
        auto n = (next_entry_offset - first_entry_offset) / entry_size;
        write16(number_of_entries_offset, (std::uint_least16_t)n);
        filesize = pixdata_offset + bytecount;
    }
    
    void make_rgb24(std::uint_least16_t width, std::uint_least16_t height)
    {
        std::uint_least16_t samples = 3;
        std::uint_least32_t bytecount =
                                (std::uint_least32_t)width * height * samples;
        auto next_entry_offset = first_entry_offset;
        create_short_field(next_entry_offset, tiff_width_tag, width);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_length_tag, height);
        next_entry_offset += entry_size;
        create_nshort_field(next_entry_offset, bps_offset, tiff_bps_tag,
                            {8, 8, 8});
        next_entry_offset += entry_size;
        create_short_field( next_entry_offset, tiff_compression_tag,
                            tiff_nocompression);
        next_entry_offset += entry_size;
        create_short_field( next_entry_offset, tiff_photometric_tag, tiff_rgb);
        next_entry_offset += entry_size;
        create_long_field(  next_entry_offset, tiff_stripoffsets_tag,
                            pixdata_offset);
        next_entry_offset += entry_size;
        create_short_field
                        (next_entry_offset, tiff_samplesperpixel_tag, samples);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_rowsperstrip_tag, height);
        next_entry_offset += entry_size;
        create_long_field(  next_entry_offset, tiff_stripbytecounts_tag,
                            bytecount);
        next_entry_offset += entry_size;
        create_rational_field(  next_entry_offset, xres_offset, tiff_xres_tag,
                                72, 1);
        next_entry_offset += entry_size;
        create_rational_field(  next_entry_offset, yres_offset, tiff_yres_tag,
                                72, 1);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_resunit_tag, tiff_inch);
        next_entry_offset += entry_size;
        write32(next_entry_offset, tiff_lastifd);
        auto n = (next_entry_offset - first_entry_offset) / entry_size;
        write16(number_of_entries_offset, (std::uint_least16_t)n);
        filesize = pixdata_offset + bytecount;
    }
    
    void make_rgb32(std::uint_least16_t width, std::uint_least16_t height)
    {
        std::uint_least16_t samples = 4;
        std::uint_least32_t bytecount =
                                (std::uint_least32_t)width * height * samples;
        auto next_entry_offset = first_entry_offset;
        create_short_field(next_entry_offset, tiff_width_tag, width);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_length_tag, height);
        next_entry_offset += entry_size;
        create_nshort_field(next_entry_offset, bps_offset, tiff_bps_tag,
                            {8, 8, 8, 8});
        next_entry_offset += entry_size;
        create_short_field( next_entry_offset, tiff_compression_tag,
                            tiff_nocompression);
        next_entry_offset += entry_size;
        create_short_field( next_entry_offset, tiff_photometric_tag, tiff_rgb);
        next_entry_offset += entry_size;
        create_long_field(  next_entry_offset, tiff_stripoffsets_tag,
                            pixdata_offset);
        next_entry_offset += entry_size;
        create_short_field
                        (next_entry_offset, tiff_samplesperpixel_tag, samples);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_rowsperstrip_tag, height);
        next_entry_offset += entry_size;
        create_long_field(  next_entry_offset, tiff_stripbytecounts_tag,
                            bytecount);
        next_entry_offset += entry_size;
        create_rational_field(  next_entry_offset, xres_offset, tiff_xres_tag,
                                72, 1);
        next_entry_offset += entry_size;
        create_rational_field(  next_entry_offset, yres_offset, tiff_yres_tag,
                                72, 1);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_resunit_tag, tiff_inch);
        next_entry_offset += entry_size;
        create_short_field(next_entry_offset, tiff_extrasamples_tag, 0);
        next_entry_offset += entry_size;
        write32(next_entry_offset, tiff_lastifd);
        auto n = (next_entry_offset - first_entry_offset) / entry_size;
        write16(number_of_entries_offset, (std::uint_least16_t)n);
        filesize = pixdata_offset + bytecount;
    }
    
private:
    std::vector<unsigned char> bytes;
    std::size_t filesize = pixdata_offset;

    static constexpr std::size_t header_size = 8;
    static constexpr std::size_t number_of_entries_offset = header_size;
    static constexpr std::size_t number_of_entries_size = 2;
    static constexpr std::size_t first_entry_offset =
                                        header_size + number_of_entries_size;
    static constexpr std::size_t max_entries = 13;
    static constexpr std::size_t entry_size = 12;
    static constexpr std::size_t lastifdmark_size = 4;
    static constexpr std::size_t xres_offset =
            first_entry_offset + max_entries * entry_size + lastifdmark_size;
    static constexpr std::size_t yres_offset = xres_offset + 8;
    static constexpr std::size_t resinfo_size = 16;
    static constexpr std::size_t bps_offset = yres_offset + 8;
    static constexpr std::size_t bps_size = 8;
    static constexpr std::size_t colormap_size8 = 3 * 256;
    static constexpr std::size_t colormap_size4 = 3 * 16;
    static constexpr std::size_t colormap_bytesize8 = 2 * colormap_size8;
    static constexpr std::size_t colormap_bytesize4 = 2 * colormap_size4;
    static constexpr std::size_t pixdata_padding = 2;
    static constexpr std::size_t pixdata_offset = first_entry_offset +
        max_entries * entry_size + lastifdmark_size + resinfo_size + bps_size +
        pixdata_padding + colormap_bytesize8;
    static constexpr std::size_t colormap_offset =
                                        pixdata_offset - colormap_bytesize8;

    void create_short_field
        (std::size_t pos, std::uint_least16_t tag, std::uint_least16_t value)
    {
        write16(pos, tag);
        write16(pos + 2, tiff_short);
        write32(pos + 4, 1);
        write16(pos + 8, value);
        write16(pos + 10, 0);
    }
    
    void
    create_nshort_field(std::size_t pos, std::size_t datapos,
                        std::uint_least16_t tag,
                        std::span<const std::uint_least16_t> val)
    {
        write16(pos, tag);
        write16(pos + 2, tiff_short);
        write32(pos + 4, val.size());
        write32(pos + 8, datapos);
        for(auto i = val.begin(); i != val.end(); ++i, datapos+= 2)
            write16(datapos, *i);
    }

    void
    create_nshort_field(std::size_t pos, std::size_t datapos,
                        std::uint_least16_t tag,
                        const std::initializer_list<std::uint_least16_t>& val)
    {
        write16(pos, tag);
        write16(pos + 2, tiff_short);
        write32(pos + 4, val.size());
        write32(pos + 8, datapos);
        for(auto i = val.begin(); i != val.end(); ++i, datapos+= 2)
            write16(datapos, *i);
    }

    void create_long_field
        (std::size_t pos, std::uint_least16_t tag, std::uint_least32_t value)
    {
        write16(pos, tag);
        write16(pos + 2, tiff_long);
        write32(pos + 4, 1);
        write32(pos + 8, value);
    }
    
    void create_rational_field( std::size_t pos, std::size_t datapos,
                                std::uint_least16_t tag,
                                std::uint_least32_t numerator,
                                std::uint_least32_t denominator)
    {
        write16(pos, tag);
        write16(pos + 2, tiff_rational);
        write32(pos + 4, 1);
        write32(pos + 8, datapos);
        write32(datapos, numerator);
        write32(datapos + 4, denominator);
    }
    
};

}


#endif // faulib_format_tiff_hpp
