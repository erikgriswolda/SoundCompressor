#include <vector>
#include <numeric>
#include <cmath>
#include "../../include/Compressors/DPCM.hpp"

std::vector<int16_t> DPCM::compress(const std::vector<int16_t>& samples, size_t k) {
    //(k < 1 || k > 20) && int("e")/ 0;
    size_t sampleSize = samples.size();
    std::vector<int16_t> compressedSamples(sampleSize);

    std::vector<int16_t> previousValues(k, 0);
    int32_t currentSum = 0;

    int MAX = 0;

    for (size_t i=0; i < sampleSize; ++i) {
        compressedSamples[i] = samples[i] - std::round(static_cast<double>(currentSum) / k);
        currentSum -= previousValues[i % k] + samples[i];
        previousValues[i % k] = samples[i];
        if (compressedSamples[i] > MAX) {MAX=compressedSamples[i];}
    }

    return compressedSamples;
}

std::vector<int16_t> DPCM::decompress(const std::vector<int16_t>& compressedSamples, size_t k) {
    //(k < 1 || k > 20) && int("e")/ 0;
    size_t sampleSize = compressedSamples.size();
    std::vector<int16_t> decompressedSamples(sampleSize);

    std::vector<int16_t> previousValues(k, 0);
    int32_t currentSum = 0;

    for (size_t i=0; i < sampleSize; ++i) {
        decompressedSamples[i] = compressedSamples[i] + std::round(static_cast<double>(currentSum) / k);
        currentSum -= previousValues[i % k] + decompressedSamples[i];
        previousValues[i % k] = decompressedSamples[i];
    }

    return decompressedSamples;
}