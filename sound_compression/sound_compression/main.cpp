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


int main() {

    auto [header, samples] = WavReader::readWavFile("Jazz.wav");
    //std::vector<int16_t> thing = MuLaw::compress(samples);
    printf("%d", samples[0]);

    
    WavWriter::writeWavFile("NewJazz.wav", header, samples);
    

    
    
    return 0;
}
// run with ctrl shift B