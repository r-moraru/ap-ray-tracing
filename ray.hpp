#ifndef RAY_HPP_
#define RAY_HPP_

#include "vec3.hpp"

class Ray {
private:
    Point o;
    Vec3 d;
public:
    Ray() {}
    Ray(const Point& origin, const Vec3& direction) : o(origin), d(direction) {}

    const Point& origin() const { return o; }
    const Vec3& direction() const { return d; }

    const Point at(double t) const {
        return o + t*d;
    }
};

#endif