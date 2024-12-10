#include <fstream>
#include <vector>
#include "../include/WavReader.hpp"
#include "../include/WavHeader.hpp"

std::tuple<WavHeader, std::vector<int16_t>> WavReader::readWavFile(const std::string& filename) {
    // Opening file
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error: Could not open file " + filename);
    }

    // Reading WAV header
    WavHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (std::string(header.riff, 4) != "RIFF" || std::string(header.wave, 4) != "WAVE") {
        throw std::runtime_error("Error: Invalid WAV file format");
    }

    // Determining sample properties
    int numSamples = header.dataSize / (header.numChannels * (header.bitsPerSample / 8));

    // Read audio data
    std::vector<int16_t> samples(numSamples);
    file.read(reinterpret_cast<char*>(samples.data()), header.dataSize);
    if (!file) {
        throw std::runtime_error("Error: Could not read audio data");
    }

    // Close the file and return the header and samples
    file.close();
    return std::make_tuple(header, samples);
}