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
    // std::cout << "RIFF Header: " << std::string(header.riff, 4) << std::endl;
    // std::cout << "Chunk Size: " << header.chunkSize << std::endl;
    // std::cout << "WAVE Header: " << std::string(header.wave, 4) << std::endl;
    // std::cout << "FMT Header: " << std::string(header.fmt, 4) << std::endl;
    // std::cout << "Subchunk1 Size: " << header.subchunk1Size << std::endl;
    // std::cout << "Audio Format: " << header.audioFormat << std::endl;
    // std::cout << "Number of Channels: " << header.numChannels << std::endl;
    // std::cout << "Sample Rate: " << header.sampleRate << std::endl;
    // std::cout << "Byte Rate: " << header.byteRate << std::endl;
    // std::cout << "Block Align: " << header.blockAlign << std::endl;
    // std::cout << "Bits Per Sample: " << header.bitsPerSample << std::endl;
    // std::cout << "Data Header: " << std::string(header.data, 4) << std::endl;
    // std::cout << "Data Size: " << header.dataSize << std::endl << std::endl;

    samples = SilenceCompression::compress(samples);
    samples = DPCM::compress(samples, 4);
    std::vector<int8_t> smallerSamples = MuLaw::compress(samples);
    HuffmanCoding::compress(smallerSamples, header, compressedOutputFilename);
}

void SuperCompressor::decompress(const std::string& compressedInputFilename, const std::string& wavOutputFilename) {
    auto [header, compressedSamples] = HuffmanCoding::decompress(compressedInputFilename);
    std::vector<int16_t> samples = MuLaw::decompress(compressedSamples);
    samples = DPCM::decompress(samples, 4);

    WavWriter::writeWavFile(wavOutputFilename, header, samples);
}

