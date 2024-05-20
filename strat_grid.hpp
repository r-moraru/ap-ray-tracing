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

inline void renderGrid(Viewport &viewport, HittableList &world,
                       std::vector<std::vector<Pixel>> &image) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int dims[2] = {0, 0};
  MPI_Dims_create(size, 2, dims);

  int periods[2] = {0, 0};
  MPI_Comm cartComm;
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cartComm);

  int coords[2];
  MPI_Cart_coords(cartComm, rank, 2, coords);

  int blockHeight = viewport.imageHeight / dims[0];
  int blockWidth = viewport.imageWidth / dims[1];

  int startY = coords[0] * blockHeight;
  int startX = coords[1] * blockWidth;

  for (int i = startY; i < startY + blockHeight; i++) {
    for (int j = startX; j < startX + blockWidth; j++) {
      image[i][j] = viewport.getPixelColor(i, j, world);
    }
  }

  MPI_Comm_free(&cartComm);
}
