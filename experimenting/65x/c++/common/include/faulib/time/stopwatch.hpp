//  stopwatch.hpp
//  Stopwatch utilities
//
//  Created by Alejandro Castro García on 8 May 2020.
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


/*

# EXPLANATION OF INTENT

Because the time library in the C++ standard library is very flexible, it is
also very complex and verbose, so we prefer not to use it directly everywhere
where stopwatch functionality is needed. Instead we intend to create some
classes and functions that are simpler to use.

For now, only one kind of stopwatch is defined:

* Time_trial: allows to measure time marks for an arbitrary number of runners
and compare results for any two of them.

*/

#ifndef faulib_time_stopwatch_hpp
#define faulib_time_stopwatch_hpp

#include <chrono>
#include <vector>

namespace faulib
{
namespace time
{

struct Runner // Anything with a start time and a stop time
{
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> stop;
};

struct Runner_comparison // Result of comparison between two runners
{
    double nanoseconds_runner1;
    double nanoseconds_runner2;
    double ratio; // nanoseconds_runner2 / nanoseconds_runner1
    // ratio is redundant, but convenient
};

class Time_trial
{
public:
    Time_trial(size_t n) :
    runners(n)
    {}
    void start(size_t index)
    {
        runners.at(index).start = std::chrono::steady_clock::now();
    }
    void stop(size_t index)
    {
        runners.at(index).stop = std::chrono::steady_clock::now();
    }
    Runner_comparison compare(size_t first_index, size_t second_index)
    {
        using std::chrono::duration;
        using std::chrono::duration_cast;
        using std::chrono::nanoseconds;
        
        Runner& runner1 = runners.at(first_index);
        Runner& runner2 = runners.at(second_index);
        nanoseconds ns1 =
            duration_cast<nanoseconds>(runner1.stop - runner1.start);
        nanoseconds ns2 =
            duration_cast<nanoseconds>(runner2.stop - runner2.start);
        
        Runner_comparison comp;
        comp.nanoseconds_runner1 = ns1.count();
        comp.nanoseconds_runner2 = ns2.count();
        if (comp.nanoseconds_runner1 == 0)
        {
            // Avoid division by zero
            if (comp.nanoseconds_runner2 == 0)
                comp.ratio = 1;
            else
                comp.ratio = numeric_limits<double>::max();
        }
        else
            comp.ratio = comp.nanoseconds_runner2 / comp.nanoseconds_runner1;
        return comp;
    }
private:
    std::vector<Runner> runners;
};

} // time
} // faulib

#endif /* faulib_time_stopwatch_hpp */
