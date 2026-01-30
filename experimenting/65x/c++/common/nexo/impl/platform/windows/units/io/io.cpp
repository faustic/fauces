// io.cpp
// Input/output: implementation for Windows.
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

#include <limits>

nexo::Io_handler::Io_handler()
{
    impl = std::make_unique<Impl>();
}

std::vector<nexo::Byte> nexo::Io_handler::read(size_t data_size)
{
    vector<Byte> data(data_size);
    size_t offset = 0;
    size_t size = data_size;
    DWORD dwordmax = (std::numeric_limits<DWORD>::max)();
    DWORD tmpsize = (size <= dwordmax) ? (DWORD)size : dwordmax;
    DWORD bytes_read;
    auto org = data.data() + offset;
    if (!ReadFile(impl->h, org, tmpsize, &bytes_read, nullptr))
            throw Error_io_read();    
    while (bytes_read < size)
    {
        offset += bytes_read;
        size -= bytes_read;
        tmpsize = (size <= dwordmax) ? (DWORD)size : dwordmax;
        org = data.data() + offset;
        if (!ReadFile(impl->h, org, tmpsize, &bytes_read, nullptr))
            throw Error_io_read();    
    }
    return data;
}

nexo::Io_handler& nexo::Io_handler::operator<<(span<Byte> data)
{
    size_t size = data.size();
    size_t offset = 0;
    DWORD dwordmax = (std::numeric_limits<DWORD>::max)();
    DWORD tmpsize = (size <= dwordmax) ? (DWORD)size : dwordmax;
    DWORD bytes_written;
    auto org = data.data() + offset;
    if (!WriteFile(impl->h, org, tmpsize, &bytes_written, nullptr))
            throw Error_io_read();    
    while (bytes_written < size)
    {
        offset += bytes_written;
        size -= bytes_written;
        tmpsize = (size <= dwordmax) ? (DWORD)size : dwordmax;
        org = data.data() + offset;
        if (!WriteFile(impl->h, org, tmpsize, &bytes_written, nullptr))
            throw Error_io_read();    
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
