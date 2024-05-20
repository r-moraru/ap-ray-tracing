#include "hittable.hpp"
#include "hittable_list.hpp"
#include "image_renderer.hpp"
#include "lambertian.hpp"
#include "material.hpp"
#include "metal.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "utils.hpp"
#include "vec3.hpp"
#include "viewport.hpp"
#include <vector>

inline void renderLinear(Viewport &viewport, HittableList &world,
                         std::vector<std::vector<Pixel>> &image) {
  for (int i = 0; i < viewport.imageHeight; i++) {
    for (int j = 0; j < viewport.imageWidth; j++) {
      image[i][j] = viewport.getPixelColor(i, j, world);
    }
  }
}
