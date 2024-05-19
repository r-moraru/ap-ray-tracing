#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <limits>
#include <memory>
#include <cstdlib>
#include <random>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

double degreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

double randomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

double randomDouble(double min, double max) {
    return min + (max-min) * randomDouble();
}

#endif