// HUFFMANCODING.hpp
#ifndef HUFFMANCODING_HPP
#define HUFFMANCODING_HPP

#include <vector>
#include <cstdint>
#include <tuple>
#include <memory>
#include "HuffmanNode.hpp"
#include "../WavHeader.hpp"

class HuffmanCoding {
public:
    static void compress(const std::vector<int8_t>& samples, const WavHeader& header, const std::string& filename);
    static std::tuple<WavHeader, std::vector<int8_t>> decompress(const std::string& filename);
};

#endif // HUFFMANCODING_HPP

