#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include <vector>
#include <cmath>
#include <iostream>

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

    Vec3 operator-() { return Vec3({x, y, z}); }

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

class Sphere {
public:
    double radius;
    Point center;

    Sphere(double radius, Point center) : radius(radius), center(center) {}
};

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

    const Pixel getColor() const {
        auto t = hits(Sphere(0.5, Point({0, 0, -1})));

        if (t > 0.0) {
            Vec3 N = toUnit(at(t) - Vec3({0, 0, -1}));
            return 0.5*Pixel({N.x+1, N.y+1, N.z+1});
        }

        return Pixel({0.0, 0.0, 0.0});
    }

    double hits(Sphere s) const {
        Vec3 oc = s.center - o;
        auto a = dot(d, d);
        auto b = -2.0 * dot(d, oc);
        auto c = dot(oc, oc) - s.radius*s.radius;
        auto discriminant = b*b - 4*a*c;
        
        if (discriminant < 0) {
            return -1.0;
        } else {
            return (-b - sqrt(discriminant)) / (2.0*a);
        }
    }
};

#endif