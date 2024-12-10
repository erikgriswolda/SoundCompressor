#include <vector>
#include <cmath> 
#include "../../include/Compressors/ScalarQuantization.hpp"

std::vector<int16_t> ScalarQuantization::quantize(const std::vector<int16_t>& samples, const int16_t scalar) {
    std::vector<int16_t> quantizedSamples(samples.size());
    for (size_t i=0; i < samples.size(); ++i) {
        quantizedSamples[i] = std::round(samples[i]/scalar) * scalar;
    }
    return quantizedSamples;
}

