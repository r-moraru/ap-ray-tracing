#ifndef HITTABLE_HPP_
#define HITTABLE_HPP_

#include "vec3.hpp"
#include "interval.hpp"

class HitRecord {
public:
    Point p;
    Vec3 normal;
    double t;
    bool frontFace;

    void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& r, Interval rayT, HitRecord& rec) const = 0;
};

#endif