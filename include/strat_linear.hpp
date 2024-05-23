#include "hittable_list.hpp"
#include "vec3.hpp"
#include "viewport.hpp"
#include <mpi.h>
#include <vector>

inline void renderLinear(Viewport &viewport, HittableList &world,
                         std::vector<std::vector<Pixel>> &image) {

  for (int i = 0; i < viewport.imageHeight; i++) {
    for (int j = 0; j < viewport.imageWidth; j++) {
      image[i][j] = viewport.getPixelColor(i, j, world);
    }
  }
}
