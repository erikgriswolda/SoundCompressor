// DPCM.hpp
#ifndef DPCM_HPP
#define DPCM_HPP

#include <vector>
#include <cstdint>

class DPCM {
public:
    static std::vector<int16_t> compress(const std::vector<int16_t>& samples, size_t k);
    static std::vector<int16_t> decompress(const std::vector<int16_t>& compressedSamples, size_t k);
};

#endif // DPCM_HPP
