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

#include <ZLibCPP.hpp>
#include <ZLibCPP_helper.hpp>
#include <stdexcept>

namespace ZLibCPP {

struct impl {
  impl() {
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
  }
  z_stream strm;
};

deflate::deflate(compression_level level) : pimpl(new impl) {
  switch (deflateInit(&pimpl->strm, level)) {
  case Z_OK:
    break;
  case Z_MEM_ERROR:
    throw std::runtime_error(pimpl->strm.msg);
    break;
  case Z_STREAM_ERROR:
    throw std::runtime_error(pimpl->strm.msg);
    break;
  case Z_VERSION_ERROR:
    throw std::runtime_error(pimpl->strm.msg);
    break;
  }
}

deflate::~deflate() {
  deflateEnd(&pimpl->strm);
  delete pimpl;
}

std::vector<unsigned char>
deflate::compress(std::span<unsigned char> data) const {

  std::vector<unsigned char> out(deflateBound(&pimpl->strm, data.size_bytes()));

  pimpl->strm.avail_in = data.size_bytes();
  pimpl->strm.next_in = data.data();

  pimpl->strm.avail_out = out.size();
  pimpl->strm.next_out = out.data();

  if (::deflate(&pimpl->strm, Z_FINISH) == Z_STREAM_END) {
    if (pimpl->strm.avail_out != 0) {
      auto size = out.size() - pimpl->strm.avail_out;
      out.resize(size);
    }
  } else {
    throw std::runtime_error("deflate error!");
  }

  if (::deflateReset(&pimpl->strm) != Z_OK) {
    throw std::runtime_error("deflateReset error!");
  }

  return out;
}

std::string deflate::compress(std::string_view data) const {

  std::string out(deflateBound(&pimpl->strm, data.size()), 0);

  pimpl->strm.avail_in = data.size();
  pimpl->strm.next_in =
      reinterpret_cast<unsigned char *>(const_cast<char *>(data.data()));

  pimpl->strm.avail_out = out.size();
  pimpl->strm.next_out = reinterpret_cast<unsigned char *>(out.data());

  if (::deflate(&pimpl->strm, Z_FINISH) == Z_STREAM_END) {
    if (pimpl->strm.avail_out != 0) {
      auto size = out.size() - pimpl->strm.avail_out;
      out.resize(size);
    }
  } else {
    throw std::runtime_error("deflate error!");
  }

  if (::deflateReset(&pimpl->strm) != Z_OK) {
    throw std::runtime_error("deflateReset error!");
  }

  return out;
}

ZLibCPP::inflate::inflate() : pimpl{new impl} {
  switch (::inflateInit(&pimpl->strm)) {
  case Z_OK:
    break;
  case Z_MEM_ERROR:
    throw std::runtime_error(pimpl->strm.msg);
    break;
  case Z_STREAM_ERROR:
    throw std::runtime_error(pimpl->strm.msg);
    break;
  case Z_VERSION_ERROR:
    throw std::runtime_error(pimpl->strm.msg);
    break;
  }
}

ZLibCPP::inflate::~inflate() {
  ::inflateEnd(&pimpl->strm);
  delete pimpl;
}

std::vector<unsigned char>
ZLibCPP::inflate::decompress(std::span<unsigned char> data,
                             size_t original_size) const {

  std::vector<unsigned char> out(original_size);

  pimpl->strm.avail_in = data.size_bytes();
  pimpl->strm.next_in = data.data();
  pimpl->strm.avail_out = original_size;
  pimpl->strm.next_out = out.data();

  if (::inflate(&pimpl->strm, Z_FINISH) != Z_STREAM_END) {
    throw std::runtime_error(
        "The original_size of inflate::decompress function is wrong!");
  }

  if (::inflateReset(&pimpl->strm) != Z_OK) {
    throw std::runtime_error("deflateReset error!");
  }

  return out;
}

std::string ZLibCPP::inflate::decompress(std::string_view data,
                                         size_t original_size) const {

  std::string out(original_size, 0);

  pimpl->strm.avail_in = data.size();
  pimpl->strm.next_in =
      reinterpret_cast<unsigned char *>(const_cast<char *>(data.data()));
  pimpl->strm.avail_out = original_size;
  pimpl->strm.next_out = reinterpret_cast<unsigned char *>(out.data());

  if (::inflate(&pimpl->strm, Z_FINISH) != Z_STREAM_END) {
    throw std::runtime_error(
        "The original_size of inflate::decompress function is wrong!");
  }

  if (::inflateReset(&pimpl->strm) != Z_OK) {
    throw std::runtime_error("deflateReset error!");
  }

  return out;
}

}; // namespace ZLibCPP