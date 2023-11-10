# ZLibCPP

This is an ultra-simplistic zlib.h C++ wrapper that compresses (deflate)
```std::string_view``` and ```std::span<unsigned char>``` array and decompresses (inflate) the 
same data if its original uncompressed size was stored.

Without the original uncompressed size, it's not possible to decompress(inflate) 
the data yet.

# Usage:

```
#include <ZLibCPP.hpp>
#include <iostream>
#include <string>

int main(){
    std::string test_data("this is an test");

    ZLibCPP::deflate def(ZLibCPP::best_compression);
    auto compressed = def.compress(test_data);
    auto original_size = test_data.size();

    ZLibCPP::inflate inf;
    auto decompressed = inf.decompress(compressed, orignal_size);

    std::cout << decompressed << std::endl;

    return 0;
}
```