#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "include/WavReader.hpp"
#include "include/WavWriter.hpp"
#include "include/Compressors/ScalarQuantization.hpp"
#include "include/Compressors/DPCM.hpp"
#include "include/Compressors/HuffmanCoding.hpp"
#include "include/Compressors/HuffmanNode.hpp"
#include "include/Compressors/MuLaw.hpp"

#include "include/SuperCompressor.hpp"

int main() {

    SuperCompressor::compress("test_audios/audio/music.wav", "test_audios/compressed_audio/music.txt");
    SuperCompressor::decompress("test_audios/compressed_audio/music.txt", "test_audios/decompressed_audio/music.wav");

    SuperCompressor::compress("test_audios/audio/crowd.wav", "test_audios/compressed_audio/crowd.txt");
    SuperCompressor::decompress("test_audios/compressed_audio/crowd.txt", "test_audios/decompressed_audio/crowd.wav");

    SuperCompressor::compress("test_audios/audio/rain.wav", "test_audios/compressed_audio/rain.txt");
    SuperCompressor::decompress("test_audios/compressed_audio/rain.txt", "test_audios/decompressed_audio/rain.wav");


    return 0;
}
// run with:
// g++ -g src/WavReader.cpp src/WavWriter.cpp src/Compressors/DPCM.cpp src/Compressors/HuffmanCoding.cpp src/Compressors/ScalarQuantization.cpp src/Compressors/MuLaw.cpp src/Compressors/RunLengthEncoding.cpp src/Compressors/SilenceCompression.cpp src/SuperCompressor.cpp main.cpp -o sound_compression 