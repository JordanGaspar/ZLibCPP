/*MIT License

Copyright (c) 2023 Jordan Gaspar Alves Silva

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
SOFTWARE.*/

#ifndef ZLIBCPP_HPP_
#define ZLIBCPP_HPP_

#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace ZLibCPP {
struct impl;

//! Required level of compression.
enum compression_level {
  no_compression = 0,
  best_speed = 1,
  best_compression = 9,
  default_compression = -1
};

class deflate {
  impl *pimpl;

public:
  //! Create deflate object with compression_level
  deflate(compression_level level = default_compression);
  ~deflate();

  //! Compress(deflate) the array os unsigned char objects returning a vector of it.
  std::vector<unsigned char> compress(std::span<unsigned char> data) const;
  //! Compress(deflate) the string_view and return the compressed string.
  std::string compress(std::string_view data) const;
};

class inflate {
  impl *pimpl;

public:
  //! Construct the inflate class.
  inflate();
  ~inflate();

  //! Decompress(deflate) function that accept an array and the 
  //! original uncompressed size, returning a vector of unsigned char.
  std::vector<unsigned char> decompress(std::span<unsigned char> data,
                                        size_t original_size) const;

  //! Decompress(deflate) function that accepts a string_view and the oringal
  //! uncompressed size, returning a uncompressed string.
  std::string decompress(std::string_view data, size_t original_size) const;
};
}; // namespace ZLibCPP

#endif