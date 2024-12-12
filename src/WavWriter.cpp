#include <fstream>
#include <vector>
#include <iostream>
#include "../include/WavWriter.hpp"
#include "../include/WavHeader.hpp"

void WavWriter::writeWavFile(const std::string& filename, const WavHeader& header, const std::vector<int16_t> samples) {
    // Opening file
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error: Could not open file " + filename);
    }

    // Writing WAV header to file
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));

    // Writing audio data to file
    file.write(reinterpret_cast<const char*>(samples.data()), header.dataSize);

    // Close the file and return the header and samples
    file.close();
}