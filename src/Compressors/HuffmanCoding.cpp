#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <memory>
#include <unordered_map>
#include <functional>
#include <string>
#include "../../include/Compressors/HuffmanCoding.hpp"
#include "../../include/Compressors/HuffmanNode.hpp"
#include "../../include/WavHeader.hpp"

#include <iostream>
#include <bitset>

const int16_t NON_LEAF_MARKER = -32768; // Marker for non-leaf nodes

void generateHuffmanCodes(std::unordered_map<int8_t, std::string>& huffmanCodes, const std::string& codeSoFar, std::shared_ptr<HuffmanNode> currentNode) {
    if (!currentNode) return;

    if (currentNode->isLeaf()) {
        huffmanCodes[currentNode->value] = codeSoFar;
        return;
    }

    generateHuffmanCodes(huffmanCodes, codeSoFar + "0", currentNode->left);
    generateHuffmanCodes(huffmanCodes, codeSoFar + "1", currentNode->right);
}

void HuffmanCoding::compress(const std::vector<int8_t>& samples, const WavHeader& header, const std::string& filename) {
    // Calculate frequencies
    std::unordered_map<int8_t, uint16_t> frequencyMap;
    for (const auto& sample : samples) {
        int8_t absValue = static_cast<int8_t>(std::abs(sample));
        frequencyMap[absValue]++;
    }

    // Create initial nodes
    std::vector<std::shared_ptr<HuffmanNode>> nodes;
    for (const auto& [value, frequency] : frequencyMap) {
        nodes.push_back(std::make_shared<HuffmanNode>(frequency, value));
    }  

    // Build Huffman Tree
    while (nodes.size() > 1) {
        std::sort(nodes.begin(), nodes.end(), [](const std::shared_ptr<HuffmanNode>& a, const std::shared_ptr<HuffmanNode>& b) {
            return a->frequency < b->frequency;
        });

        auto left = nodes[0];
        auto right = nodes[1];
        nodes.erase(nodes.begin(), nodes.begin() + 2);

        auto parent = std::make_shared<HuffmanNode>(left->frequency + right->frequency, NON_LEAF_MARKER);
        parent->left = left;
        parent->right = right;

        nodes.push_back(parent);
    }

    auto treeRoot = nodes.front();

    // Generate Huffman codes
    std::unordered_map<int8_t, std::string> huffmanCodes;
    generateHuffmanCodes(huffmanCodes, "", treeRoot);

    // Encode samples into bitstream
    std::vector<uint8_t> bitstream(1, 0); // First byte for leftover bit count
    uint8_t currentByte = 0;
    size_t bitCount = 0;

    for (const auto& sample : samples) {
        int8_t absValue = std::abs(sample);
        std::string code = huffmanCodes[static_cast<int16_t>(absValue)];
        std::string signBit = (sample < 0) ? "1" : "0";

        // Write sign bit
        currentByte = (currentByte << 1) | (signBit[0] - '0');
        ++bitCount;

        if (bitCount == 8) {
            bitstream.push_back(currentByte);
            currentByte = 0;
            bitCount = 0;
        }

        // Write Huffman code
        for (const auto& bit : code) {
            currentByte = (currentByte << 1) | (bit - '0');
            ++bitCount;

            if (bitCount == 8) {
                bitstream.push_back(currentByte);
                currentByte = 0;
                bitCount = 0;
            }
        }
    }

    if (bitCount > 0) {
        currentByte <<= (8 - bitCount);
        bitstream.push_back(currentByte);
        bitstream[0] = 8 - bitCount;
    }

    // Write to file
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Failed to open file for writing");
    }

    outFile.write(reinterpret_cast<const char*>(&header), sizeof(header));
    size_t bitstreamSize = bitstream.size();
    outFile.write(reinterpret_cast<const char*>(&bitstreamSize), sizeof(bitstreamSize));
    outFile.write(reinterpret_cast<const char*>(bitstream.data()), bitstream.size());

    std::function<void(const std::shared_ptr<HuffmanNode>&)> writeTree = [&](const std::shared_ptr<HuffmanNode>& node) {
        if (!node) {
            outFile.put(0);
            return;
        }

        outFile.put(1);
        int8_t nodeValue = node->value;
        outFile.write(reinterpret_cast<const char*>(&nodeValue), sizeof(nodeValue));
        outFile.write(reinterpret_cast<const char*>(&node->frequency), sizeof(node->frequency));
        writeTree(node->left);
        writeTree(node->right);
    };

    writeTree(treeRoot);
    outFile.close();
}

std::tuple<WavHeader, std::vector<int8_t>> HuffmanCoding::decompress(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        throw std::runtime_error("Failed to open file for reading");
    }

    WavHeader header;
    inFile.read(reinterpret_cast<char*>(&header), sizeof(header));

    size_t bitstreamSize;
    inFile.read(reinterpret_cast<char*>(&bitstreamSize), sizeof(bitstreamSize));

    std::vector<uint8_t> bitstream(bitstreamSize);
    inFile.read(reinterpret_cast<char*>(bitstream.data()), bitstreamSize);

    std::function<std::shared_ptr<HuffmanNode>()> readTree = [&]() {
        char marker;
        inFile.get(marker);

        if (marker == 0) return std::shared_ptr<HuffmanNode>(nullptr);

        auto node = std::make_shared<HuffmanNode>(0, NON_LEAF_MARKER);
        int8_t nodeValue;
        inFile.read(reinterpret_cast<char*>(&nodeValue), sizeof(nodeValue));
        node->value = nodeValue;
        inFile.read(reinterpret_cast<char*>(&node->frequency), sizeof(node->frequency));
        node->left = readTree();
        node->right = readTree();

        return node;
    };

    auto treeRoot = readTree();

    size_t unusedBits = bitstream[0];
    size_t totalBits = ((bitstream.size() - 1) * 8) - unusedBits;

    std::vector<int8_t> samples;
    auto currentNode = treeRoot;
    bool readingSign = true;
    bool isNegative = false;

    for (size_t i = 0; i < totalBits; ++i) {
        bool bit = (bitstream[1 + (i / 8)] >> (7 - (i % 8))) & 1;

        if (readingSign) {
            isNegative = bit;
            readingSign = false;
        } else {
            currentNode = bit ? currentNode->right : currentNode->left;

            if (currentNode->isLeaf()) {
                int8_t value = static_cast<int8_t>(currentNode->value);
                if (isNegative) value = -value;
                samples.push_back(value);
                currentNode = treeRoot;
                readingSign = true;
            }
        }
    }

    return {header, samples};
}
