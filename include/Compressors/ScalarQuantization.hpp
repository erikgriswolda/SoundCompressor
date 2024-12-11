// SCALARQUANTIZATION.hpp
#ifndef SCALARQUANTIZATION_HPP
#define SCALARQUANTIZATION_HPP

#include <string>
#include <vector>
#include <cstdint>

class ScalarQuantization {
public:
    static std::vector<int16_t> quantize(const std::vector<int16_t>& samples, int16_t scalar);
};

#endif // SCALARQUANTIZATION_HPP