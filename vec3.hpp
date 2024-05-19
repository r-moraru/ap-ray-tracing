#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include <vector>
#include <cmath>
#include <iostream>

#include "utils.hpp"

class Vec3 {
public:
    double x, y, z;

    Vec3() {
        x = 0, y = 0, z = 0;
    }

    Vec3(std::vector<double> vals) {
        if (vals.size() != 3) {
            return;
        }
        x = vals[0];
        y = vals[1];
        z = vals[2];
    };

    Vec3 operator-() const { return Vec3({x, y, z}); }

    Vec3& operator+=(const Vec3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vec3& operator*=(const Vec3& v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    Vec3& operator*=(double s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    Vec3& operator/=(double s) {
        return *this *= 1/s;
    }
    double lengthSquared() const {
        return x*x + y*y + z*z;
    }
    double length() const {
        return sqrt(lengthSquared());
    }
    static Vec3 random() {
        return Vec3({randomDouble(), randomDouble(), randomDouble()});
    }
    static Vec3 random(double min, double max) {
        return Vec3({randomDouble(min, max), randomDouble(min, max), randomDouble(min, max)});
    }
};

typedef Vec3 Pixel;
typedef Vec3 Point;

std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}

Vec3 operator+(const Vec3& v1, const Vec3& v2) {
    return Vec3({v1.x+v2.x, v1.y+v2.y, v1.z+v2.z});
}

Vec3 operator-(const Vec3& v1, const Vec3& v2) {
    return Vec3({v1.x-v2.x, v1.y-v2.y, v1.z-v2.z});
}

Vec3 operator*(const Vec3& v1, const Vec3& v2) {
    return Vec3({v1.x*v2.x, v1.y*v2.y, v1.z*v2.z});
}

Vec3 operator*(double t, const Vec3& v) {
    return Vec3({t*v.x, t*v.y, t*v.z});
}

Vec3 operator*(const Vec3& v, double t) {
    return t*v;
}

Vec3 operator/(const Vec3& v, double t) {
    return Vec3({1/t*v.x, 1/t*v.y, 1/t*v.z});
}

double dot(const Vec3& v1, const Vec3& v2) {
    Vec3 prod = v1*v2;
    return prod.x + prod.y + prod.z;
}

Vec3 cross(const Vec3& v1, const Vec3& v2) {
    return Vec3({v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x});
}

Vec3 toUnit(const Vec3& v) {
    return v / v.length();
}

Vec3 randomInUnitSphere() {
    while (true) {
        auto p = Vec3::random(-1, 1);
        if (p.lengthSquared() < 1) {
            return p;
        }
    }
}

Vec3 randomUnitVector() {
    return toUnit(randomInUnitSphere());
}

Vec3 randomOnHemisphere(const Vec3& normal) {
    Vec3 onUnitSphere = randomUnitVector();
    if (dot(onUnitSphere, normal) > 0.0) {
        return onUnitSphere;
    } else {
        return -onUnitSphere;
    }
}

#endif