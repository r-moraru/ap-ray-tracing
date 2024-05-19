#include <vector>

#include "image_renderer.hpp"
#include "viewport.hpp"
#include "vec3.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "utils.hpp"
#include "sphere.hpp"
#include "ray.hpp"

using namespace std;

int main() {
    double aspectRatio = 19.0 / 16.0;
    int imageWidth = 600;

    Viewport viewport(aspectRatio, imageWidth);

    vector<vector<Pixel>> image(viewport.imageHeight, vector<Pixel>(viewport.imageWidth));

    HittableList world;
    world.add(make_shared<Sphere>(Point({0, 0, -1}), 0.5));
    world.add(make_shared<Sphere>(Point({0, -100.5,-1}), 100));

    for (int i = 0; i < viewport.imageHeight; i++) {
        for (int j = 0; j < viewport.imageWidth; j++) {
            image[i][j] = viewport.getPixelColor(i, j, world);
        }
    }

    toPpmFile(image, "test.ppm");
}