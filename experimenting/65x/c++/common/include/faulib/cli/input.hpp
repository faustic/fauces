//  Utilities to deal with console input
//
//  Created by Alejandro Castro García on 25 May 2022.
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



#ifndef faulib_cli_input_hpp
#define faulib_cli_input_hpp

#include <iostream>
#include <thread>
#include <mutex>
#include <cstdlib>

namespace faulib::cli
{

using std::string;
using std::mutex;
using std::lock_guard;
using std::condition_variable;
using std::unique_lock;
using std::shared_ptr;
using std::make_shared;

struct Command_reader_control
// Information to be shared between the thread that created the Command_reader
// structure and the thread created by the Command_reader constructor
{
    bool is_destroyed = false;
    string command;
    mutex command_mutex;
    mutex destruct_mutex;
    condition_variable ready;
};

class Command_reader
// An interruptible command reader. Depending on the C++ implementation, it may
// lead to  undefined behavior.
// Assumptions we are making:
//  - cout and cin are not destroyed while there are still threads running.
//  - if std::exit is called while cin is waiting for input in another thread,
//    things will happen in this order:
//      1. Some global object will be destroyed that will cause the thread
//         waiting for input to be destroyed, effectively ending the wait.
//      2. Global variables cin and cout will be destroyed after the previous
//         step is complete.
// If one of these assumptions is false, then we may face one of the following
// problems:
//  - that cin or cout are accessed after they have been destroyed, causing
//    undefined behavior.
//  - that cin keeps waiting forever even after std::exit is called, unless
//    the quit command is entered.
{
public:
    Command_reader(const string& quit_cmd)
    // Creates a thread that will manage the input loop, and detaches it
    {
        control = make_shared<Command_reader_control>();
        std::thread t(input_loop, control, quit_cmd);
        t.detach();
    }
    
    string read()
    // Reads the last command read by the input loop, and clears it.
    // This function is expected to be called from the same thread that created
    // this Command_reader. The sleep helps keep power consumption within
    // limits
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(100ms);
        string cmd;
        {
            lock_guard<mutex> lock {control->command_mutex};
            cmd = control->command;
            control->command.clear();
        }
        control->ready.notify_one();
        return cmd;
    }
    
    ~Command_reader()
    // Tries to make the input loop aware that this Command_reader has ceased
    // to exist
    {
        try
        {
            lock_guard<mutex> lock {control->destruct_mutex};
            control->is_destroyed = true;
        }
        catch(const std::exception& err)
        {
            std::cerr << err.what() << "\n";
        }
        try
        {
            {
                lock_guard<mutex> lock {control->command_mutex};
                control->command.clear();
            }
            control->ready.notify_one();
        }
        catch(const std::exception& err)
        {
            std::cerr << err.what() << "\n";
        }
    }
    
private:
    shared_ptr<Command_reader_control> control;

    static void input_loop( shared_ptr<Command_reader_control> control,
                            const string quit_cmd)
    // The input loop runs inside its own detached thread. The control
    // structure is a shared_ptr so it remains valid even after
    // the Command_reader object has been destroyed. After reading a line, a
    // wait is kept until the command just read is cleared by another thread
    // before trying to read another one. The loop ends after a quit command or
    // if destruction of the Command_reader object is detected.
    {
        using namespace std::chrono_literals;
        string cmd;
        do
        {
            std::cout << "$ ";
            std::getline(std::cin, cmd);  // Danger: what if cin was destroyed?
                                          // We assume the current thread is
                                          // killed before that happens.
            if (cmd.length())
            {
                lock_guard<mutex> d_lock {control->destruct_mutex};
                if (control->is_destroyed)
                    break;
                unique_lock<mutex> c_lock {control->command_mutex};
                control->command = cmd;
                while (control->command.length())
                    control->ready.wait_for(c_lock, 100ms);
            }
        } while (cmd != quit_cmd);
    }
    
};

} // faulib::cli


#endif /* faulib_cli_input_hpp */
