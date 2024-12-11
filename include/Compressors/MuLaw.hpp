// MuLaw.hpp
#ifndef MULAW_HPP
#define MULAW_HPP

#include <vector>
#include <cstdint>

class MuLaw {
public:
    static std::vector<int8_t> compress(const std::vector<int16_t>& samples);
    static std::vector<int16_t> decompress(const std::vector<int8_t>& compressedSamples);
};

#endif // MULAW_HPP
