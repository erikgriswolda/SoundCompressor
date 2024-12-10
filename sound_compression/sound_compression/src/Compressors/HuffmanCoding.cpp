#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <memory>
#include <unordered_map>
#include "../../include/Compressors/HuffmanCoding.hpp"
#include "../../include/Compressors/HuffmanNode.hpp"

#include <iostream>
#include <bitset>

void generateHuffmanCodes(std::unordered_map<int16_t, std::string>& huffmanCodes, const std::string& codeSoFar, std::shared_ptr<HuffmanNode> currentNode) {
    if (!currentNode) return;

    if (currentNode->isLeaf()) {
        // std::cout << currentNode->value << " " << codeSoFar << std::endl;
        huffmanCodes[currentNode->value] = codeSoFar;
        return;
    }

    generateHuffmanCodes(huffmanCodes, codeSoFar + "0", currentNode->left);
    generateHuffmanCodes(huffmanCodes, codeSoFar + "1", currentNode->right);
}

std::tuple<std::vector<uint8_t>, std::shared_ptr<HuffmanNode>> HuffmanCoding::compress(const std::vector<int16_t>& samples) {
    // Sorting the samples
    std::vector<int16_t> sortedSamples = samples;
    std::sort(sortedSamples.begin(), sortedSamples.end());

    // Count frequencies
    int16_t current_value = sortedSamples[0];
    uint16_t current_count = 1;
    std::vector<std::pair<uint16_t, int16_t>> sampleCounts;
    for (size_t i = 1; i < sortedSamples.size(); ++i) {
        if (current_value != sortedSamples[i]) {
            sampleCounts.emplace_back(current_count, current_value);
            current_value = sortedSamples[i];
            current_count = 0;
        }
        ++current_count;
    }
    sampleCounts.emplace_back(current_count, current_value);

    // Build initial nodes
    std::vector<std::shared_ptr<HuffmanNode>> nodes;
    for (const auto& [freq, value] : sampleCounts) {
        nodes.push_back(std::make_shared<HuffmanNode>(freq, value));
    }
    std::cout <<"length="<<nodes.size()<<std::endl;

    // Build the Huffman Tree
    while (nodes.size() > 1) {
        std::nth_element(nodes.begin(), nodes.begin() + 1, nodes.end(), [](const std::shared_ptr<HuffmanNode>& a, const std::shared_ptr<HuffmanNode>& b) {
            return a->frequency < b->frequency;
        });

        auto left = std::move(nodes[0]);
        auto right = std::move(nodes[1]);
        nodes.erase(nodes.begin(), nodes.begin() + 2);

        int16_t combinedFrequency = left->frequency + right->frequency;
        nodes.push_back(std::make_shared<HuffmanNode>(combinedFrequency, std::move(left), std::move(right)));

        std::push_heap(nodes.begin(), nodes.end(), [](const std::shared_ptr<HuffmanNode>& a, const std::shared_ptr<HuffmanNode>& b) {
            return a->frequency > b->frequency;
        });
    }
    std::shared_ptr<HuffmanNode> treeRoot = std::move(nodes[0]);

    // Generate Huffman Codes
    std::unordered_map<int16_t, std::string> huffmanCodes;
    generateHuffmanCodes(huffmanCodes, "", treeRoot);

    // Encode samples into a bitstream
    std::vector<uint8_t> bitstream(1, 0);  // First byte for leftover bit count
    uint8_t current_byte = 0;
    size_t currentBitIndex = 0;

    for (size_t i = 0; i < samples.size(); ++i) {
        int16_t sampleValue = samples[i];
        std::string valueSign = (sampleValue < 0) ? "1" : "0";
        sampleValue = std::abs(sampleValue);

        std::string currentSampleCode = valueSign + huffmanCodes[sampleValue];
        for (char bit : currentSampleCode) {
            current_byte = (current_byte << 1) | (bit - '0');
            ++currentBitIndex;

            if (currentBitIndex == 8) {
                bitstream.push_back(current_byte);
                current_byte = 0;
                currentBitIndex = 0;
            }
        }
    }

    // Add remaining bits
    if (currentBitIndex > 0) {
        current_byte <<= (8 - currentBitIndex);
        bitstream.push_back(current_byte);
        bitstream[0] = 8 - currentBitIndex; 
    }

    // Debugging: Print the bitstream
    // for (uint8_t val : bitstream) {
    //     std::cout << std::bitset<8>(val);
    // }
    // std::cout << bitstream.size();

    return std::make_tuple(bitstream, std::move(treeRoot));
}

std::vector<int16_t> HuffmanCoding::decompress(const std::vector<uint8_t>& compressedSamples, std::shared_ptr<HuffmanNode> treeRoot) {
    std::vector<int16_t> decompressedSamples;

    if (compressedSamples.empty() || !treeRoot) {
        return decompressedSamples; 
    }

    size_t unusedBitsInLastByte = compressedSamples[0];
    size_t totalBits = ((compressedSamples.size() - 1) * 8) - unusedBitsInLastByte;

    std::shared_ptr<HuffmanNode> currentNode = treeRoot;

    bool currentSignIsNegative;
    bool atSignIndex = true;
    for (size_t bit = 0; bit < totalBits; ++bit) {
        bool currentBit = (compressedSamples[1 + (bit / 8)] >> (7 - (bit % 8))) & 1;

        if (atSignIndex) {
            currentSignIsNegative = currentBit;
            atSignIndex = false;
        } else {
            if (!currentBit) {
                currentNode = currentNode->left;
            } else {
                currentNode = currentNode->right;
            }

            if (currentNode->isLeaf()) {
                int16_t value = currentNode->value;
                if (currentSignIsNegative) {value *= -1;}
                decompressedSamples.push_back(value);
                currentNode = treeRoot;
                atSignIndex = true;
            }
        }
    }

    return decompressedSamples;
}
