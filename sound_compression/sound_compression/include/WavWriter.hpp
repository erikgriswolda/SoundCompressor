// WavWriter.hpp
#ifndef WAVWRITER_HPP
#define WAVWRITER_HPP

#include <string>
#include <vector>
#include <tuple>
#include "WavHeader.hpp"

class WavWriter {
public:
    static void writeWavFile(const std::string& filename, const WavHeader& header, const std::vector<int16_t> data);
};

#endif // WAVWRITER_HPP
