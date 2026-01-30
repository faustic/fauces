// serial.cpp
// Serial communications: implementation for Windows
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

#include "io_windows.hpp"
#include <nexo/io/serial.hpp>

namespace nexo
{

static DCB& operator<<(DCB& dcb, Databits databits);
static DCB& operator<<(DCB& termios, Stopbits stopbits);
static DCB& operator<<(DCB& dcb, Parity parity);
static DCB& operator<<(DCB& dcb, Baudrate bauds);
}

static DCB& nexo::operator<<(DCB& dcb, Databits databits)
{
    switch (databits)
    {
        case Databits::five:
            dcb.ByteSize = 5;
            break;
        case Databits::six:
            dcb.ByteSize = 6;
            break;
        case Databits::seven:
            dcb.ByteSize = 7;
            break;
        case Databits::eight:
            dcb.ByteSize = 8;
            break;
    }
    return dcb;
}

static DCB& nexo::operator<<(DCB& dcb, Stopbits stopbits)
{
    if (stopbits == Stopbits::two)
        dcb.StopBits = TWOSTOPBITS;
    else
        dcb.StopBits = ONESTOPBIT;
    return dcb;
}

static DCB& nexo::operator<<(DCB& dcb, Parity parity)
{
    switch (parity)
    {
        case Parity::no:
            dcb.fParity = false;
            dcb.Parity = NOPARITY;
            break;
        case Parity::even:
            dcb.fParity = true;
            dcb.Parity = EVENPARITY;
            break;
        case Parity::odd:
            dcb.fParity = true;
            dcb.Parity = ODDPARITY;
            break;
    }
    return dcb;
}

static DCB& nexo::operator<<(DCB& dcb, Baudrate bauds)
{
    dcb.BaudRate = bauds;
    return dcb;
}

namespace
{
inline nexo::Error_io_serial_internal cant_get_attr()
{
    return nexo::Error_io_serial_internal("Cannot get port attributes");
}

inline nexo::Error_io_serial_internal cant_set_attr()
{
    return nexo::Error_io_serial_internal("Cannot set port attributes");
}
}

nexo::Serialport::Serialport
                    (const string& port_name, const Serial_options* options)
{
    impl->h = CreateFileA(  port_name.c_str(), GENERIC_READ | GENERIC_WRITE,
                            0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                            nullptr);
    if (impl->h == INVALID_HANDLE_VALUE)
        throw Error_io_open();
    if (options)
        set_options(*options);
}

nexo::Serialport::~Serialport()
{
    if (impl->h != INVALID_HANDLE_VALUE)
    {
        CloseHandle(impl->h);
        impl->h = INVALID_HANDLE_VALUE;
    }
}

void nexo::Serialport::set_options(const Serial_options& options)
{
    DCB dcb {sizeof(DCB)};
    if (!GetCommState(impl->h, &dcb))
        throw cant_get_attr();
    dcb << options.bauds << options.databits << options.stopbits <<
        options.parity;
    if (!SetCommState(impl->h, &dcb))
        throw cant_set_attr();
}

void nexo::Serialport::set_dtr()
{
    DCB dcb {sizeof(DCB)};
    if (!GetCommState(impl->h, &dcb))
        throw cant_get_attr();
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    if (!SetCommState(impl->h, &dcb))
        throw nexo::Error_io_serial_internal("Cannot set DTR");
}

void nexo::Serialport::clear_dtr()
{
    DCB dcb {sizeof(DCB)};
    if (!GetCommState(impl->h, &dcb))
        throw cant_get_attr();
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    if (!SetCommState(impl->h, &dcb))
        throw nexo::Error_io_serial_internal("Cannot clear DTR");
}


