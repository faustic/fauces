// serial.cpp
// Serial communications: implementation for POSIX
//
// Created by Alejandro Castro García on 1 September 2021
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

#include "io_posix.hpp"
#include <nexo/io/serial.hpp>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

namespace nexo
{
static termios& operator<<(termios& termios, Databits databits);

static termios& operator<<(termios& termios, Stopbits stopbits);
static termios& operator<<(termios& termios, Parity parity);

static termios& operator<<(termios& termios, speed_t bauds);

}

static termios& nexo::operator<<(termios& termios, Databits databits)
{
    termios.c_cflag &= ~CSIZE;
    switch (databits)
    {
        case Databits::five:
            termios.c_cflag |= CS5;
            break;
        case Databits::six:
            termios.c_cflag |= CS6;
            break;
        case Databits::seven:
            termios.c_cflag |= CS7;
            break;
        case Databits::eight:
            termios.c_cflag |= CS8;
            break;
    }
    return termios;
}

static termios& nexo::operator<<(termios& termios, Stopbits stopbits)
{
    if (stopbits == Stopbits::two)
        termios.c_cflag |= CSTOPB;
    else
        termios.c_cflag &= ~CSTOPB;
    return termios;
}

static termios& nexo::operator<<(termios& termios, Parity parity)
{
    switch (parity)
    {
        case Parity::no:
            termios.c_cflag &= ~PARENB;
            break;
        case Parity::even:
            termios.c_cflag &= ~PARODD;
            termios.c_cflag |= PARENB;
            break;
        case Parity::odd:
            termios.c_cflag |= (PARENB | PARODD);
            break;
    }
    return termios;
}

static termios& nexo::operator<<(termios& termios, speed_t bauds)
{
    if (cfsetspeed(&termios, bauds))
        throw Error_io_serial_baudrate();
    return termios;
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
// Note that the user must belong to the "dialout" group or permission
// to open the serial port will be denied
    impl->fd = open(port_name.c_str(), O_RDWR|O_NOCTTY);
    if (impl->fd <= 0)
        throw Error_io_open();
    termios termios;
    if (tcgetattr(impl->fd, &termios))
        throw cant_get_attr();
    termios.c_iflag = 0;
    termios.c_oflag = 0;
    termios.c_cflag = HUPCL | CLOCAL | CREAD;
    termios.c_lflag = 0;
    termios.c_cc[VMIN] = 0;
    termios.c_cc[VTIME] = 0;
    if (tcsetattr(impl->fd, TCSANOW, &termios))
        throw cant_set_attr();
    if (options)
        set_options(*options);
}

nexo::Serialport::~Serialport()
{
    if (impl->fd > 0)
    {
        close(impl->fd);
        impl->fd = -1;
    }
}

void nexo::Serialport::set_options(const Serial_options& options)
{
    termios termios;
    if (tcgetattr(impl->fd, &termios))
        throw cant_get_attr();
    termios << options.bauds << options.databits << options.stopbits <<
        options.parity;
    if (tcsetattr(impl->fd, TCSANOW, &termios))
        throw cant_set_attr();
    if (tcgetattr(impl->fd, &termios))
        throw cant_get_attr();
}

void nexo::Serialport::set_dtr()
{
    int flags = TIOCM_DTR;
    if (ioctl(impl->fd, TIOCMBIS, &flags))
        throw nexo::Error_io_serial_internal("Cannot set DTR");
}

void nexo::Serialport::clear_dtr()
{
    int flags = TIOCM_DTR;
    if (ioctl(impl->fd, TIOCMBIC, &flags))
        throw nexo::Error_io_serial_internal("Cannot clear DTR");
}


