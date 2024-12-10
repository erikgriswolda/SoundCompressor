// HUFFMANCODING.hpp
#ifndef HUFFMANCODING_HPP
#define HUFFMANCODING_HPP

#include <vector>
#include <cstdint>
#include <tuple>
#include <memory>
#include "HuffmanNode.hpp"

class HuffmanCoding {
public:
    static std::tuple<std::vector<uint8_t>, std::shared_ptr<HuffmanNode>> compress(const std::vector<int16_t>& samples);
    static std::vector<int16_t> decompress(const std::vector<uint8_t>& compressedSamples, std::shared_ptr<HuffmanNode> treeRoot);
};

#endif // HUFFMANCODING_HPP

