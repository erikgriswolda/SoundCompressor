// SUPERCOMRESSOR.hpp
#ifndef SUPERCOMRESSOR_HPP
#define SUPERCOMRESSOR_HPP

#include <vector>
#include <string>

class SuperCompressor {
public:
    static void compress(const std::string& wavInputFilename, const std::string& compressedOutputFilename);
    static void decompress(const std::string& compressedInputFilename, const std::string& wavOutputFilename);
};

#endif // SUPERCOMRESSOR_HPP

