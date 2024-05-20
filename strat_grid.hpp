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
#include <string>
#include <vector>
enum Strategy { HORIZONTAL, GRID };

typedef struct {
  double r, g, b;
} Color;

inline void renderGrid(Viewport &viewport, HittableList &world,
                       std::vector<std::vector<Pixel>> &image,
                       Strategy strategy, bool loadBalanced) {
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int imageHeight = viewport.imageHeight;
  int imageWidth = viewport.imageWidth;

  int dims[2] = {0, 0};
  MPI_Dims_create(size, 2, dims);

  int periods[2] = {0, 0};
  MPI_Comm cartComm;
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cartComm);
  MPI_Comm_rank(cartComm, &rank);

<<<<<<< HEAD
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
=======

  int numWorkers = size;
  int rowOffset = rank;
  int numProcessedRows = imageHeight/numWorkers;
  if (imageHeight%numWorkers > rowOffset) {
    numProcessedRows += 1;
  }

  int localPixelsSize = 3 * imageWidth * numProcessedRows;
  int imgSize = 3 * imageWidth * imageHeight;

  double *localPixels = (double *)malloc(sizeof(double) * localPixelsSize);
  double *img;
  if (rank == 0) {
    img = (double *)malloc(sizeof(double) * imgSize);
  }

  std::cout << "num workers: " << size << std::endl;
  std::cout << "row offset: " << rank << std::endl;
  std::cout << "num processed rows: " << numProcessedRows << std::endl;
  std::cout << "local pixels size: " << imageWidth * numProcessedRows << std::endl;
  std::cout << "total image size: " << imageHeight*imageWidth << std::endl;
  
  int currentPixel = 0;
  for (int i = rowOffset; i < imageHeight; i += numWorkers) {
    // std::cout << i << std::endl;
    for (int j = 0; j < imageWidth; j++, currentPixel += 3) {
      Pixel p = viewport.getPixelColor(i, j, world);
      localPixels[currentPixel] = p.x;
      localPixels[currentPixel+1] = p.y;
      localPixels[currentPixel+2] = p.z;
    }
    // std::cout << "current pixel: " << currentPixel << std::endl;
  }

  int numBytesRead = MPI_Gather((void *)localPixels, localPixelsSize, MPI_DOUBLE, (void *)img, localPixelsSize, MPI_DOUBLE, 0, cartComm);

  if (rank == 0) {
    currentPixel = 0;
    for (int i = 0; i < imageHeight; i++) {
      for (int j = 0; j < imageWidth; j++, currentPixel += 3) {
        image[i][j] = Pixel({
          img[currentPixel],
          img[currentPixel+1],
          img[currentPixel+2]
        });
        // std::cout << image[i][j] << std::endl;
      }
>>>>>>> refs/remotes/origin/main
    }

  toPpmFile(localImage, std::to_string(rank) + "test.ppm");
  MPI_Comm_free(&cartComm);
}
