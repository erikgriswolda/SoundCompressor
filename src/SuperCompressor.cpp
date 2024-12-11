#include <vector>
#include <numeric>
#include <iostream>
#include <cmath>

#include "../include/SuperCompressor.hpp"

#include "../include/WavReader.hpp"
#include "../include/WavWriter.hpp"
#include "../include/Compressors/SilenceCompression.hpp"
#include "../include/Compressors/DPCM.hpp"
#include "../include/Compressors/MuLaw.hpp"
#include "../include/Compressors/HuffmanCoding.hpp"

void SuperCompressor::compress(const std::string& wavInputFilename, const std::string& compressedOutputFilename) {
    auto [header, samples] = WavReader::readWavFile(wavInputFilename);

    samples = SilenceCompression::compress(samples);
    samples = DPCM::compress(samples, 3);
    std::vector<int8_t> smallerSamples = MuLaw::compress(samples);
    HuffmanCoding::compress(smallerSamples, header, compressedOutputFilename);
}

void SuperCompressor::decompress(const std::string& compressedInputFilename, const std::string& wavOutputFilename) {
    auto [header, compressedSamples] = HuffmanCoding::decompress(compressedInputFilename);
    std::vector<int16_t> samples = MuLaw::decompress(compressedSamples);
    samples = DPCM::decompress(samples, 3);

    WavWriter::writeWavFile(wavOutputFilename, header, samples);
}