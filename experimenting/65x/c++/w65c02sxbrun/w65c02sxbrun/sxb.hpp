// W65C02SXB communications
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


#ifndef w65c02sxb_hpp
#define w65c02sxb_hpp

#include <stdexcept>
#include <climits>

#include <nexo/io/serial.hpp>


namespace w65c02
{
using nexo::Byte;
using nexo::vector;
using nexo::span;
using nexo::string;
using nexo::array;
using nexo::Serialport;
using nexo::Databits;
using nexo::Stopbits;
using nexo::Parity;
using std::runtime_error;
using std::logic_error;

class Error_sxb_badsizeformat: public logic_error
{
public:
    Error_sxb_badsizeformat(): logic_error("w65c02 Error_sxb_badsizeformat") {}
};

class Error_sxb_badreply: public  runtime_error
{
public:
    Error_sxb_badreply(): runtime_error("w65c02 Error_sxb_badreply") {}
};

class Error_sxb_datatoobig: public logic_error
{
public:
    Error_sxb_datatoobig(): logic_error("w65c02 Error_sxb_datatoobig") {}
};

class Error_sxb_unexpectedboard: public runtime_error
{
public:
    Error_sxb_unexpectedboard():
    runtime_error("w65c02 Error_sxb_unexpectedboard") {}
};

enum class Command : Byte
{
    sync = 0,
    echo = 1,
    from_pc = 2,
    to_pc = 3,
    board_info = 4,
    exec = 5
};

class Sxb;

class Address
{
    friend Sxb;
public:
    Address(unsigned long address) :  value {address}
    {}
    
    vector<Byte> get_bytes()
    {
        vector<Byte> bytes;
        bytes.push_back(value & 0xff);
        bytes.push_back((value >> 8) & 0xff);
        bytes.push_back((value >> 16) & 0xff);
        return bytes;
    }
    
    operator unsigned long()
    {
        return value;
    }
    
private:
    unsigned long value;
};

class Address16
{
    friend Sxb;

public:
    Address16(unsigned address) :  value {address}
    {}

    explicit Address16(Address address)
    {
        value = static_cast<unsigned>(address);
    }

    explicit Address16(span<Byte> address)
    {
        value = 0;
        if (address.size() > 0)
        {
            value = address[0];
            if (address.size() > 1)
            value |= address[1] << 8;
        }
    }

    vector<Byte> get_bytes()
    {
        vector<Byte> bytes;
        bytes.push_back(value & 0xff);
        bytes.push_back((value >> 8) & 0xff);
        return bytes;
    }
    
    operator unsigned()
    {
        return value;
    }
    
    operator Address()
    {
        return Address(value);
    }
    
private:
    unsigned value;
};

class Size
{
    friend Sxb;
public:
    Size(unsigned size) :  value {size}
    {}
    
    Size(span<Byte> bytes)
    {
        if (bytes.size() != 2)
            throw Error_sxb_badsizeformat();
        value = bytes[0] & (bytes[1] << 8);
    }
    
    operator unsigned()
    {
        return value;
    }
    
    vector<Byte> get_bytes()
    {
        vector<Byte> bytes;
        bytes.push_back(value & 0xff);
        bytes.push_back((value >> 8) & 0xff);
        return bytes;
    }
    
private:
    unsigned value;
};

enum class Board_type
{
    w65c02sxb,
    w65c816sxb,
    unknown
};

class Sxb
{
public:
    Sxb(const string& port_name) :
    port {port_name}
    {
        port.set_options({115200, Databits::eight, Stopbits::one,
                            Parity::no});
        vector<Byte> info = board_info();
        if (is_65c02(info))
            type = Board_type::w65c02sxb;
        else if (is_65c816(info))
            type = Board_type::w65c816sxb;
        else
            throw Error_sxb_unexpectedboard();
    }
    
    bool is_65c02()
    {
        return type == Board_type::w65c02sxb;
    }

    bool is_65c816()
    {
        return type == Board_type::w65c816sxb;
    }

    static bool is_65c02(vector<Byte>& info)
    {
        return info[3] == 0 && info[4] == 0x58;
    }
    
    static bool is_65c816(vector<Byte>& info)
    {
        return info[3] == 1 && info[4] == 0x58;
    }
    
    Sxb& operator<<(const span<Byte>& data)
    {
        port << data;
        return *this;
    }
    
    Sxb& operator<<(Byte data)
    {
        port << data;
        return *this;
    }
    
    Sxb& operator<<(Command command)
    {
        array<Byte, 2> start = {0x55, 0xaa};
        port << start;
        vector<Byte> response = port.read(1);
        if (response.size() != 1 || response[0] != 0xcc)
            throw Error_sxb_badreply();
        port << static_cast<Byte>(command);
        return *this;
    }
    
    Sxb& operator<<(Address address)
    {
        auto bytes = address.get_bytes();
        return (*this << bytes);
    }
    
    Sxb& operator<<(Size size)
    {
        auto bytes = size.get_bytes();
        return (*this << bytes);
    }
    
    Sxb& operator>>(Size& size)
    {
        vector<Byte> bytes = port.read(2);
        size = Size {bytes};
        return *this;
    }
    
    Sxb& operator>>(Byte& byte)
    {
        port >> byte;
        return *this;
    }
    
    vector<Byte> board_info()
    {
        *this << Command::board_info;
        std::vector<Byte> response = port.read(29);
        return response;
    }
    
    void from_pc(Address addr, span<Byte> bytes)
    {
        if (bytes.size() > UINT_MAX)
            throw Error_sxb_datatoobig();
        unsigned size = static_cast<unsigned>(bytes.size());
        *this << Command::from_pc << addr << Size(size);
        *this << bytes;
    }
    
    vector<Byte> to_pc(Address addr, Size size)
    {
        *this << Command::to_pc << addr << size;
        vector<Byte> bytes = port.read(size);
        return bytes;
   }
    
    void run(Address addr)
    {
        vector<Byte> addr_bytes = addr.get_bytes();
        array<Byte, 16> state =
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, addr_bytes[0], addr_bytes[1],
            0x00, 0x00, 0xff, 0xff, 0x30, 0x01, 0x00, 0x00
        };
        from_pc(Address {0x7e00}, state);
        *this << Command::exec;
        wait_for_break();
    }
    
    void wait_for_break()
    {
        // From trial and error we believe that the board sends a single byte
        // after encountering a BRK instruction. The byte seems to always be
        // 0x02, but it is not clear if other values are also possible.
        // For now, we will simply ignore the value.
        Byte byte;
        *this >> byte;
    }
    
private:
    Serialport port;
    Board_type type = Board_type::unknown;
};

} // namespace w65c02

#endif /* w65c02sxb_hpp */
