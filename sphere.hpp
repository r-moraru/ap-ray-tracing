#ifndef SPHERE_HPP_
#define SPHERE_HPP_

#include "hittable.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "material.hpp"

class Sphere : public Hittable {
private:
    Point center;
    double radius;
    shared_ptr<Material> mat;
public:
    Sphere(const Point& center, double radius, shared_ptr<Material> mat) : center(center), radius(fmax(0, radius)), mat(mat) {}

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
        Vec3 oc = center - r.origin();
        auto a = r.direction().lengthSquared();
        auto h = dot(r.direction(), oc);
        auto c = oc.lengthSquared() - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0) {
            return false;
        }

        auto sqrtd = sqrt(discriminant);

        auto root = (h - sqrtd) / a;
        if (!rayT.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!rayT.surrounds(root)) {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        Vec3 outwardNormal = (rec.p - center) / radius;
        rec.setFaceNormal(r, outwardNormal);
        rec.mat = mat;

        return true;
    }
};

#endif