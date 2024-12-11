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

    SuperCompressor::compress("Jazz.wav", "compressedJazz.txt");
    SuperCompressor::decompress("compressedJazz.txt", "NewJazz.wav");

    return 0;
}
// run with ctrl shift B