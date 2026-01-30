// io.cpp
// Input/output: implementation for POSIX.
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

#include <fcntl.h>
#include <unistd.h>

nexo::Io_handler::Io_handler()
{
    impl = std::make_unique<Impl>();
}

std::vector<nexo::Byte> nexo::Io_handler::read(size_t data_size)
{
    vector<Byte> data(data_size);
    size_t offset = 0;
    ssize_t size = data_size;
    ssize_t bytes_read;
    while ((bytes_read = ::read(impl->fd, data.data() + offset, size)) < size)
    {
        if (bytes_read < 0)
            throw Error_io_read();
        offset += bytes_read;
        size -= bytes_read;
    }
    return data;
}

nexo::Io_handler& nexo::Io_handler::operator<<(span<Byte> data)
{
    ssize_t size = data.size();
    size_t offset = 0;
    ssize_t bytes_written;
    while ((bytes_written = write(impl->fd, data.data() + offset, size)) < size)
    {
        if (bytes_written < 0)
            throw Error_io_write();
        offset += bytes_written;
        size -= bytes_written;
    }
    return *this;
}

nexo::Io_handler& nexo::Io_handler::operator<<(Byte byte)
{
    array<Byte, 1> data= {byte};
    return *this << data;
}

nexo::Io_handler& nexo::Io_handler::operator>>(Byte& byte)
{
    vector<Byte> bytes = read(1);
    byte = bytes.at(0);
    return *this;
}
