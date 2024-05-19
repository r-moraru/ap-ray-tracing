#include <vector>

#include "image_renderer.hpp"
#include "viewport.hpp"
#include "vec3.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "utils.hpp"
#include "sphere.hpp"
#include "ray.hpp"
#include "material.hpp"
#include "lambertian.hpp"
#include "metal.hpp"

using namespace std;

int main() {
    double aspectRatio = 16.0 / 9.0;
    int imageWidth = 900;

    Viewport viewport(aspectRatio, imageWidth);

    vector<vector<Pixel>> image(viewport.imageHeight, vector<Pixel>(viewport.imageWidth));

    HittableList world;

    auto groundMaterial = make_shared<Lambertian>(Pixel({0.8, 0.8, 0.0}));
    auto centerMaterial = make_shared<Lambertian>(Pixel({0.1, 0.2, 0.5}));
    auto leftMaterial = make_shared<Metal>(Pixel({0.8, 0.8, 0.8}));
    auto rightMaterial = make_shared<Metal>(Pixel({0.8, 0.3, 0.2}));

    world.add(make_shared<Sphere>(Point({0.0, -100.5, -1.0}), 100.0, groundMaterial));
    world.add(make_shared<Sphere>(Point({0.0, 0.0, -1.5}), 0.5, centerMaterial));
    world.add(make_shared<Sphere>(Point({-1.0, 0.0, -1.0}), 0.5, leftMaterial));
    world.add(make_shared<Sphere>(Point({1.0, 0.0, -1.0}), 0.5, rightMaterial));

    for (int i = 0; i < viewport.imageHeight; i++) {
        for (int j = 0; j < viewport.imageWidth; j++) {
            image[i][j] = viewport.getPixelColor(i, j, world);
        }
    }

    toPpmFile(image, "test.ppm");
}