#ifndef LAMBERTIAN_HPP_
#define LAMBERTIAN_HPP_

#include <iostream>

#include "material.hpp"
#include "hittable.hpp"

class Lambertian : public Material {
private:
    Pixel albedo;
public:
    Lambertian(const Pixel& albedo) : albedo(albedo) {}

    bool scatter(const Ray& rIn, const HitRecord& rec, Pixel& attenuation, Ray& scattered) const override {
        auto scatterDirection = rec.normal + randomUnitVector();\

        if (scatterDirection.nearZero()) {
            scatterDirection = rec.normal;
        }

        scattered = Ray(rec.p, scatterDirection);

        // std::cout << "scattered ray: " << scattered.origin() << " " << scattered.direction() << std::endl;
        
        attenuation = albedo;
        return true;
    }
};

#endif