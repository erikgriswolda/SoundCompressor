#include <vector>
#include <numeric>
#include <iostream>
#include <cmath>
#include "../../include/Compressors/MuLaw.hpp"

float sgn(float val) {
    return (float(0) < val) - (val < float(0));
}

std::vector<int8_t> MuLaw::compress(const std::vector<int16_t>& samples) {
    size_t sampleSize = samples.size();
    std::vector<int8_t> compressedSamples(sampleSize);


    float μ = 255.0;

    for (size_t i=0; i < sampleSize; ++i) {
        //shrink it to range of -1 to 1, number is max valkue of int_16t
        float x = static_cast<float>(samples[i]) / 32768.0;
        x = sgn(x) * (log(1.0+μ*std::fabs(x)) / log(1.0+μ));
        
        compressedSamples[i] = static_cast<int8_t>(std::round(x * 127.0));
    }

    return compressedSamples;
}

std::vector<int16_t> MuLaw::decompress(const std::vector<int8_t>& compressedSamples) {
    size_t sampleSize = compressedSamples.size();
    std::vector<int16_t> decompressedSamples(sampleSize);

    int μ = 255;

    for (size_t i=0; i < sampleSize; ++i) {
        float x = static_cast<float>(compressedSamples[i]) / 127.0;
        x = sgn(x) * ((powf((1+μ), std::fabs(x)) - 1) / μ);

        decompressedSamples[i] = static_cast<int16_t>(x * 32768.0);

    }

    return decompressedSamples;
}