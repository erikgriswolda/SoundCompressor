#include <vector>
#include "../../include/Compressors/SilenceCompression.hpp"


std::vector<int16_t> SilenceCompression::compress(const std::vector<int16_t>& samples) {
    size_t sampleSize = samples.size();
    std::vector<int16_t> compressedSamples(sampleSize, -32768);

    int silenceValue = -32768 + 300;
    for (size_t i=0; i < sampleSize; ++i) {
        if (samples[i] > silenceValue)
        {

            compressedSamples[i] = samples[i];
        }
    }

    return compressedSamples;
}

