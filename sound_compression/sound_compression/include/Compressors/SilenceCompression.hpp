// DPCM.hpp
#ifndef SILENCECOMPRESSION_HPP
#define SILENCECOMPRESSION_HPP

#include <vector>
#include <cstdint>

class SilenceCompression {
public:
    static std::vector<int16_t> compress(const std::vector<int16_t>& samples);
};

#endif // SILENCECOMPRESSION_HPP
