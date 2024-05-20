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
#include <string>
#include <vector>
enum Strategy { HORIZONTAL, GRID };

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

  int rowsPerProcess = imageHeight / size;
  int remainder = imageHeight % size;
  int startRow = rank * rowsPerProcess;
  int numRows = rowsPerProcess + (rank < remainder ? 1 : 0);
  int endRow = startRow + numRows;

  std::vector<std::vector<Pixel>> localImage(endRow - startRow,
                                             std::vector<Pixel>(imageWidth));

  for (int i = startRow; i < endRow; i++)
    for (int j = 0; j < viewport.imageWidth; j++) {
      localImage[i - startRow][j] = viewport.getPixelColor(i, j, world);
    }

  toPpmFile(localImage, std::to_string(rank) + "test.ppm");
  MPI_Comm_free(&cartComm);
}
