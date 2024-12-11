// WavReader.hpp
#ifndef WAVREADER_HPP
#define WAVREADER_HPP

#include <string>
#include <vector>
#include <tuple>
#include "WavHeader.hpp"

class WavReader {
public:
    static std::tuple<WavHeader, std::vector<int16_t>> readWavFile(const std::string& filename);
};

#endif // WAVREADER_HPP
