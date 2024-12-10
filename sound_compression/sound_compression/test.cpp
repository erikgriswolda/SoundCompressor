#include <vector>
#include <numeric>
#include <iostream>
#include <cmath>

float sgn(float val) {
    return (float(0) < val) - (val < float(0));
}
// float absolv(float man){
// man &= ~(1<<31);
// }

std::vector<int16_t> compress(const std::vector<int16_t>& samples) {
    size_t sampleSize = samples.size();
    std::vector<int16_t> compressedSamples(sampleSize);



    float μ = 255.0;

    for (size_t i=0; i < sampleSize; ++i) {
        //shrink it to range of -1 to 1, number is max valkue of int_16t
        float x = static_cast<float>(samples[i]) / 32768.0 * 2.0 - 1.0;
        x = sgn(x) * (log(1.0+μ*std::fabs(x)) / log(1.0+μ));
        
        compressedSamples[i] = x *255.0;
    }

    return compressedSamples;
}

std::vector<int16_t> decompress(const std::vector<int16_t>& compressedSamples) {
    size_t sampleSize = compressedSamples.size();
    std::vector<int16_t> decompressedSamples(sampleSize);

    int μ = 255;

    for (size_t i=0; i < sampleSize; ++i) {
        float x = compressedSamples[i] / 255.0;
        x = sgn(x) * ((powf((1+μ), std::fabs(x)) - 1) / μ);

        decompressedSamples[i] = (x + 1) * 32768 / 2;

    }

    return decompressedSamples;
}

int main() {
    std::vector<int16_t> samples = {32767, 0, 2361, 22, 10293};
    std::vector<int16_t> compressedSamples = compress(samples);
    std::vector<int16_t> decompressedSamples = decompress(compressedSamples);

    for (size_t i=0; i < samples.size(); i++) {
        std::cout << samples[i] << " " << decompressedSamples[i] << std::endl;
    }
}