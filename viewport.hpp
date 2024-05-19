#ifndef VIEWPORT_HPP_
#define VIEWPORT_HPP_

#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"

const Pixel getRayColor(const Ray& r, const Hittable& world) {
    HitRecord rec;
    if (world.hit(r, Interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + Pixel({1, 1, 1}));
    }

    return Pixel({0.0, 0.0, 0.0});
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

    Viewport(double aspectRatio, int imageWidth) : aspectRatio(aspectRatio), imageWidth(imageWidth) {
        imageHeight = int(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        viewportHeight = 2.0;
        viewportWidth = viewportHeight * (double(imageWidth)/imageHeight);

        focalLength = 1.0;
        cameraCenter = Point({0, 0, 0});

        viewportU = Vec3({viewportWidth, 0, 0});
        viewportV = Vec3({0, -viewportHeight, 0});

        pixelDeltaU = viewportU / imageWidth;
        pixelDeltaV = viewportV / imageHeight;

        viewportUpperLeft = cameraCenter - Vec3({0, 0, focalLength}) - viewportU/2 - viewportV/2;
        pixel00Center = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
    }

    const Pixel getPixelColor(int i, int j, Hittable& world) const {
        Vec3 pixelCenter = pixel00Center +
                (i * pixelDeltaV) + (j * pixelDeltaU);
        Vec3 rayDirection = pixelCenter - cameraCenter;
        Ray r(cameraCenter, rayDirection);

        Pixel pixel = getRayColor(r, world);
        return pixel;
    }
};

#endif