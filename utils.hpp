#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

double degreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

#endif