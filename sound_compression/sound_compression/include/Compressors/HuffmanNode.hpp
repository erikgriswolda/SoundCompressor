// HUFFMANNODE.hpp
#ifndef HUFFMANNODE_HPP
#define HUFFMANNODE_HPP

#include <cstdint>
#include <memory>

struct HuffmanNode {
    int16_t frequency;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    int16_t value;

    HuffmanNode(int16_t freq, int16_t val) 
        : frequency(freq), left(nullptr), right(nullptr), value(val) {}

    HuffmanNode(int16_t freq, std::shared_ptr<HuffmanNode> l, std::shared_ptr<HuffmanNode> r) 
        : frequency(freq), left(std::move(l)), right(std::move(r)), value(-1) {}

    bool isLeaf() const {
        return (left == nullptr && right == nullptr);
    }
};

#endif // HUFFMANNODE_HPP
