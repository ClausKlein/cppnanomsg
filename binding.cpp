/*
    Copyright (c) 2013 250bpm s.r.o.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom
    the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#include <nanomsg/nn.hpp>
#include <nanomsg/pair.h>  // NN_PAIR

#include <array>
#include <cassert>
#include <iostream>
#include <string_view>

int main() {
  constexpr std::string_view data("A\000BC\n");
  std::array<char, 16> buf{};

  nn::socket s1(AF_SP, NN_PAIR);
  s1.bind("inproc://a");

  {
    nn::socket s2(AF_SP, NN_PAIR);
    s2.connect("inproc://a");

    s2.send(data.data(), data.length(), 0);

    int rc = s1.recv(buf.data(), buf.size(), 0);

    assert(rc == data.length());                       // NOLINT(hicpp-no-array-decay)
    assert(data == std::string_view(buf.data(), rc));  // NOLINT(hicpp-no-array-decay)

    try {
      s2.send(data.data(), data.length(), 0);
      s2.shutdown(0);
    } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }
  }

  int rc = s1.recv(buf.data(), buf.size(), 0);  // FIXME: this waits forever! CK

  return 0;
}
