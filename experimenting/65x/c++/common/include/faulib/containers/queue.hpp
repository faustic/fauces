//  queue.hpp
//  Containers with queue-like behaviour
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

/*
 # EXPLANATION OF INTENT

 Here we define queue classes whose behaviour is not exactly the same as any
 standard library classes.

 * `Autopop_queue`: similar to `std::queue`, but a maximum size is defined on
    construction and elements are popped automatically when pushing elements
    above that limit. If the elements are of a trivial type, no allocations
    or deallocations are performed when pushing or popping elements.


*/

#include <vector>

namespace faulib::containers
{

template<typename T>
class Autopop_queue
{
public:
    using value_type = T;
    using size_type = typename std::vector<T>::size_type;
    using reference = value_type&;
    using const_reference = const value_type&;

    constexpr Autopop_queue(size_type max_size):
    values(max_size)
    {
    }

    const_reference front() const
    {
        return values[start];
    }

    reference front()
    {
        return values[start];
    }

    const_reference back() const
    {
        return values[(start + count - 1) % values.size()];
    }

    reference back()
    {
        return values[(start + count - 1) % values.size()];
    }

    [[nodiscard]] bool empty() const
    {
        return count == 0;
    }

    size_type size() const
    {
        return count;
    }

    void push(const value_type& value)
    {
        values[push_pos()] = value;
    }

    void push(value_type&& value)
    {
        values[push_pos()] = std::move(value);
    }

    void pop()
    {
        values[start] = T{};
        start = (start + 1) % values.size();
        --count;
    }

private:
    std::vector<T> values;
    size_type start = 0;
    size_type count = 0;

    size_type push_pos()
    {
        size_type pos = (start + count) % values.size();
        if (count < values.size())
            ++count;
        else
            start = (pos + 1) % values.size();
        return pos;
    }
};

}
