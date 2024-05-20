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
#include <mpi.h>
#include <vector>

inline void renderLinear(Viewport &viewport, HittableList &world,
                         std::vector<std::vector<Pixel>> &image) {

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  for (int i = 0; i < viewport.imageHeight; i++) {
    for (int j = 0; j < viewport.imageWidth; j++) {
      image[i][j] = viewport.getPixelColor(i, j, world);
    }
  }

  if (rank == 0) {
    toPpmFile(image, "test.ppm");
  }
}
