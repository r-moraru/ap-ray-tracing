#ifndef VIEWPORT_HPP_
#define VIEWPORT_HPP_

#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "material.hpp"

const Pixel getRayColor(const Ray& r, int depth, const Hittable& world) {
    if (depth <= 0) {
        return Pixel({0, 0, 0});
    }
    HitRecord rec;
    if (world.hit(r, Interval(0.001, infinity), rec)) {
        Ray scattered;
        Pixel attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered)) {
            return attenuation * getRayColor(scattered, depth-1, world);
        }
        return Pixel({0, 0, 0});
    }

    Vec3 unitDirection = toUnit(r.direction());
    auto a = 0.5*(unitDirection.y + 1.0);
    return (1.0-a)*Pixel({1.0, 1.0, 1.0}) + a*Pixel({0.5, 0.7, 1.0});
}

class Viewport {
public:
    double aspectRatio;
    int imageWidth, imageHeight;
    double viewportHeight, viewportWidth;
    double focalLength;
    Point cameraCenter;
    Vec3 viewportU, viewportV;
    Vec3 pixelDeltaU, pixelDeltaV;
    Vec3 viewportUpperLeft, pixel00Center;
    int samplesPerPixel;
    int maxDepth;
    double pixelSamplesScale;

    Viewport(double aspectRatio, int imageWidth) : aspectRatio(aspectRatio), imageWidth(imageWidth), samplesPerPixel(10), maxDepth(50) {
        imageHeight = int(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        viewportHeight = 2.0;
        viewportWidth = viewportHeight * (double(imageWidth)/imageHeight);

        focalLength = 3;
        cameraCenter = Point({0, 0.25, 2});

        viewportU = Vec3({viewportWidth, 0, 0});
        viewportV = Vec3({0, -viewportHeight, 0});

        pixelDeltaU = viewportU / imageWidth;
        pixelDeltaV = viewportV / imageHeight;

        viewportUpperLeft = cameraCenter - Vec3({0, 0, focalLength}) - viewportU/2 - viewportV/2;
        pixel00Center = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

        pixelSamplesScale = 1.0 / samplesPerPixel;
    }

    const Pixel getPixelColor(int i, int j, Hittable& world) const {
        Pixel pixel({0, 0, 0});
        for (int sample = 0; sample < samplesPerPixel; sample++) {
            Ray r = getRay(i, j);
            pixel += getRayColor(r, maxDepth, world);
        }
        return pixelSamplesScale * pixel;
    }

    Vec3 sampleSquare() const {
        return Vec3({randomDouble() - 0.5, randomDouble() - 0.5, 0});
    }

    Ray getRay(int j, int i) const {
        auto offset = sampleSquare();
        auto pixelSample = pixel00Center + ((i + offset.x) * pixelDeltaU) +
                                           ((j + offset.y) * pixelDeltaV);

        auto rayOrigin = cameraCenter;
        auto rayDirection = pixelSample - rayOrigin;

        return Ray(rayOrigin, rayDirection);
    }
};

#endif