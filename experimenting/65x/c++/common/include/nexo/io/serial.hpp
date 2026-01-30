// serial.hpp
// Serial communications: interface
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


#ifndef nexo_serial_hpp
#define nexo_serial_hpp

#include <string>
#include <cstdint>

#include <nexo/io/io.hpp>

namespace nexo
{

using std::string;

using Baudrate = std::uint_least32_t;

class Error_io_serial_baudrate: public runtime_error
{
public:
    Error_io_serial_baudrate(): runtime_error("Nexo Error_io_serial_baudrate")
    {}
};

class Error_io_serial_internal: public runtime_error
{
public:
    Error_io_serial_internal(const string& msg):
    runtime_error("Nexo Error_io_serial_internal: " + msg)
    {}
};

class Serialport;

enum class Databits {five = 5, six, seven, eight};
enum class Stopbits {one = 1, two};
enum class Parity {no, even, odd};

class Serial_options
{
    friend Serialport;
public:
    Serial_options(Baudrate bauds, Databits databits, Stopbits stopbits,
                  Parity parity):
    bauds {bauds},
    databits {databits},
    stopbits {stopbits},
    parity {parity}
    {}
    
private:
    Baudrate bauds;
    Databits databits;
    Stopbits stopbits;
    Parity parity;
};

class Serialport: public Io_handler
{
public:
    Serialport(const string& port, const Serial_options* options = nullptr);
    ~Serialport();
    void set_options(const Serial_options& options);
    void set_dtr();
    void clear_dtr();
};



} // namespace nexo

#endif /* nexo_serial_hpp */
