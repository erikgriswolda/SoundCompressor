#include <vector>
#include "../../include/Compressors/SilenceCompression.hpp"

int sgn(int16_t val) {
    return (int16_t(0) < val) - (val < int16_t(0));
}

std::vector<int16_t> SilenceCompression::compress(const std::vector<int16_t>& samples) {
    size_t sampleSize = samples.size();
    std::vector<int16_t> compressedSamples(sampleSize,0);

    int silenceValue = 100;
    for (size_t i=0; i < sampleSize; ++i) {
        if (samples[i] > silenceValue)
        {

            compressedSamples[i] = samples[i];
        }
    }

    return compressedSamples;
}

