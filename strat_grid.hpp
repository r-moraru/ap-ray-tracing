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
#include <format>
#include <mpi.h>
#include <vector>
enum Strategy { HORIZONTAL, VERTICAL, GRID };

inline void renderGrid(Viewport &viewport, HittableList &world,
                       std::vector<std::vector<Pixel>> &image,
                       Strategy strategy, bool loadBalanced) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int imageHeight = viewport.imageHeight;
  int imageWidth = viewport.imageWidth;

  int dims[2] = {0, 0};
  MPI_Dims_create(size, 2, dims);

  int periods[2] = {0, 0};
  MPI_Comm cartComm;
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cartComm);

  int coords[2];
  MPI_Cart_coords(cartComm, rank, 2, coords);
  int blockHeight = imageHeight / dims[0];
  int blockWidth = imageWidth / dims[1];

  int startY = coords[0] * blockHeight;
  int startX = coords[1] * blockWidth;

  printf("Rank:%d Size:%d, Coords(%d,%d), Dims(%d,%d)\n", rank, size, coords[0],
         coords[1], dims[0], dims[1]);

  std::vector<std::vector<Pixel>> localImage(
      blockHeight, std::vector<Pixel>(blockWidth, Pixel({0, 0, 0})));

  int nParts = loadBalanced ? size * 4 : size;
  int partHeight = imageHeight / nParts;
  int partWidth = imageWidth / nParts;

  std::vector<std::vector<Pixel>> globalImage(imageHeight,
                                              std::vector<Pixel>(imageWidth));

  if (rank == 0) {
    for (int i = 0; i < imageHeight; ++i) {
      for (int j = 0; j < imageWidth; ++j) {
        image[i][j] = globalImage[i][j];
      }
    }
  }

  MPI_Comm_free(&cartComm);
}

