#include <vector>
#include <numeric>
#include <iostream>
#include <cmath>
#include "../../include/Compressors/RunLengthEncoding.hpp"

int sgn(int16_t val) {
    return (int16_t(0) < val) - (val < int16_t(0));
}

std::vector<std::vector<int16_t>> compress(const std::vector<int16_t>& samples) {
    size_t sampleSize = samples.size();

    std::vector<int16_t> compressedSamples(sampleSize);
    // error if signal repeats more tahn 2^16 times
    std::vector<int16_t> compressedRepetitions(sampleSize);

    int16_t whatever = 1;
    compressedSamples[0] = samples[0];
    compressedRepetitions[0] =1 ;

    for (size_t i=1; i < sampleSize; ++i) {
        if (samples[i] == compressedSamples[i-whatever])
        {
            compressedRepetitions[i-whatever] += 1;
        }
        else{
            compressedSamples[i + 1 - whatever] = samples[i];
            compressedRepetitions[i + 1 - whatever] = 1;
        }
        
    }
    std::vector<std::vector<int16_t>> compressed(2);
    compressed[0] = compressedSamples;
    compressed[1] = compressedRepetitions;
    return compressed;
}

std::vector<int16_t> decompress(const std::vector<std::vector<int16_t>>& compressed) {
    size_t sampleSize = compressed[0].size();
    std::vector<int16_t> decompressedSamples(sampleSize);

    int μ = 255;
    int k=0;
    int16_t otherTHing = 0;
    for (size_t i=0; i < sampleSize; ++i) {
        for (size_t j=0; j < compressed[1][i]; i++)
        {
            decompressedSamples[k] = compressed[0][i];
            k++;

        }
        
    }

    return decompressedSamples;
}