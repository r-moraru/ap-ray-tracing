#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include "utils.hpp"
#include "ray.hpp"
#include "hittable.hpp"

class HitRecord;

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(
        const Ray& rIn, const HitRecord& rec, Pixel& attenuation, Ray& scattered) const {
            return false;
        }
};

#endif