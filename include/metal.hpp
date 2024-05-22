#ifndef METAL_HPP_
#define METAL_HPP_

#include <iostream>

#include "material.hpp"
#include "hittable.hpp"

class Metal : public Material {
private:
    Pixel albedo;
public:
    Metal(const Pixel& albedo) : albedo(albedo) {}

    bool scatter(const Ray& rIn, const HitRecord& rec, Pixel& attenuation, Ray& scattered) const override {
        Vec3 reflected = reflect(rIn.direction(), rec.normal);
        scattered = Ray(rec.p, reflected);
        // std::cout << "scattered ray: " << scattered.origin() << " " << scattered.direction() << std::endl;
        attenuation = albedo;
        return true;
    }
};

#endif