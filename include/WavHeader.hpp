// WavHeader.hpp
#ifndef WAVHEADER_HPP
#define WAVHEADER_HPP

#include <cstdint>
#include <cstring>

struct WavHeader {
    char riff[4];                 // consistent: "RIFF"
    uint32_t chunkSize;           // derived: total bytes in file minus first 8       20 + dataSize + subchunk1Size
    char wave[4];                 // consistent: "WAVE"
    char fmt[4];                  // consistent: "fmt "
    uint32_t subchunk1Size;       // consistent (mostly): typically 16 for PCM
    uint16_t audioFormat;         // consistent: 1 for uncompressed PCM
    uint16_t numChannels;         // essential: 1 for mono / 2 for stereo
    uint32_t sampleRate;          // essential: number of audio samples per second
    uint32_t byteRate;            // derived: number of bytes per second              sampleRate * numChannels * (bitsPerSample / 8)) 
    uint16_t blockAlign;          // derived: number of bytes representing 1 frame    numChannels * (bitsPerSample / 8))
    uint16_t bitsPerSample;       // essential: number of bits per sample
    char data[4];                 // consistent: "data"
    uint32_t dataSize;            // essential: combined size of all samples

    void initialize(uint32_t numChannelsValue, uint32_t sampleRateValue, uint32_t bitsPerSampleValue, uint32_t dataSizeValue) {
        std::memcpy(riff, "RIFF", 4);
        chunkSize = 20 + dataSizeValue + subchunk1Size;
        std::memcpy(wave, "WAVE", 4);
        std::memcpy(fmt, "fmt ", 4);
        subchunk1Size = 16;
        audioFormat = 1;
        numChannels = numChannelsValue;
        sampleRate = sampleRateValue;
        byteRate = sampleRateValue * numChannelsValue * (bitsPerSampleValue / 8);
        blockAlign = numChannelsValue * (bitsPerSampleValue / 8);
        bitsPerSample = bitsPerSampleValue;
        std::memcpy(data, "data", 4);
        dataSize = dataSizeValue;
    }
};


#endif // WAVHEADER_HPP
