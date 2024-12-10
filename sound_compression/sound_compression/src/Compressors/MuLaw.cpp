#include <vector>
#include <numeric>
#include <iostream>
#include <cmath>
#include "../../include/Compressors/MuLaw.hpp"

int sgn(int16_t val) {
    return (int16_t(0) < val) - (val < int16_t(0));
}

std::vector<int16_t> Mulaw::compress(const std::vector<int16_t>& samples) {
    size_t sampleSize = samples.size();
    std::vector<int16_t> compressedSamples(sampleSize);



    int μ = 255;

    for (size_t i=0; i < sampleSize; ++i) {
        //shrink it to range of -1 to 1, number is max valkue of int_16t
        compressedSamples[i] = samples[i] / 32768.0
        compressedSamples[i] = sgn(compressedSamples[i]) * (log(1+μ*abs(compressedSamples[i])) / log(1+μ));
    }

    return compressedSamples;
}

std::vector<int16_t> Mulaw::decompress(const std::vector<int16_t>& compressedSamples) {
    size_t sampleSize = compressedSamples.size();
    std::vector<int16_t> decompressedSamples(sampleSize);

    int μ = 255;

    for (size_t i=0; i < sampleSize; ++i) {
        compressedSamples[i] = samples[i] / 255.0
        decompressedSamples[i] = sgn(compressedSamples[i]) * ((pow((1+μ), abs(compressedSamples[i])) - 1) / μ);
    }

    return decompressedSamples;
}