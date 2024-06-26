#include "hittable_list.hpp"
#include "vec3.hpp"
#include "viewport.hpp"
#include <mpi.h>
#include <vector>

enum Topology { GRID, RING, LINEAR };

inline void ringInitializeMPI(int &rank, int &size, MPI_Comm &ringComm) {
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int dims[1] = {0};
  MPI_Dims_create(size, 1, dims);

  int periods[1] = {1};
  MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periods, 1, &ringComm);
  MPI_Comm_rank(ringComm, &rank);
}

inline void gridInitializeMPI(int &rank, int &size, MPI_Comm &gridComm) {
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int dims[2] = {0, 0};
  MPI_Dims_create(size, 2, dims);

  int periods[2] = {0, 0};
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &gridComm);
  MPI_Comm_rank(gridComm, &rank);
}

inline void fillLocalPixels(Viewport &viewport, HittableList &world,
                            int rowOffset, int endRow, int numWorkers,
                            int imageWidth, double *localPixels) {
  int currentPixel = 0;
  for (int i = rowOffset; i < endRow; i += numWorkers) {
    for (int j = 0; j < imageWidth; j++, currentPixel += 3) {
      Pixel p = viewport.getPixelColor(i, j, world);
      localPixels[currentPixel] = p.x;
      localPixels[currentPixel + 1] = p.y;
      localPixels[currentPixel + 2] = p.z;
    }
  }
}

inline void gatherPixels(int localPixelsSize, double *localPixels, double *img,
                         MPI_Comm &cartComm) {
  MPI_Gather((void *)localPixels, localPixelsSize, MPI_DOUBLE, (void *)img,
             localPixelsSize, MPI_DOUBLE, 0, cartComm);
}

inline void constructImage(int imageHeight, int imageWidth, double *img,
                           std::vector<std::vector<Pixel>> &image,
                           bool loadBalanced, int size) {
  int currentPixel = 0;
  std::vector<std::vector<Pixel>> tempImage(imageHeight,
                                            std::vector<Pixel>(imageWidth));

  for (int i = 0; i < imageHeight; i++) {
    for (int j = 0; j < imageWidth; j++, currentPixel += 3) {
      tempImage[i][j] = Pixel(
          {img[currentPixel], img[currentPixel + 1], img[currentPixel + 2]});
    }
  }

  int currentRow = 0;
  image = tempImage;
  if (loadBalanced) {
    for (int i = 0; i < size; i++) {
      int numProcessedRows = imageHeight / size;
      if (imageHeight % size > i) {
        numProcessedRows += 1;
      }
      for (int j = 0; j < numProcessedRows; j++, currentRow++) {
        image[i + j * size] = tempImage[currentRow];
      }
    }
  }
}

inline void renderGrid(Viewport &viewport, HittableList &world,
                       std::vector<std::vector<Pixel>> &image,
                       Topology topology, bool loadBalanced) {
  int rank, size;
  MPI_Comm cartComm;

  if (topology == RING) {
    ringInitializeMPI(rank, size, cartComm);
  } else {
    gridInitializeMPI(rank, size, cartComm);
  }

  int imageHeight = viewport.imageHeight;
  int imageWidth = viewport.imageWidth;

  int numWorkers = size;
  int rowOffset = rank;
  int numProcessedRows = imageHeight / numWorkers;
  if (imageHeight % numWorkers > rowOffset) {
    numProcessedRows += 1;
  }

  int localPixelsSize = 3 * imageWidth * numProcessedRows;
  int imgSize = 3 * imageWidth * imageHeight;

  double *localPixels = nullptr;
  double *img = nullptr;

  localPixels = (double *)malloc(sizeof(double) * localPixelsSize);
  if (rank == 0) {
    img = (double *)malloc(sizeof(double) * imgSize);
  }

  if (!loadBalanced) {
    rowOffset = rank * numProcessedRows;
    numWorkers = 1;
  }

  int endRow = loadBalanced ? imageHeight : rowOffset + numProcessedRows;
  fillLocalPixels(viewport, world, rowOffset, endRow, numWorkers, imageWidth,
                  localPixels);

  gatherPixels(localPixelsSize, localPixels, img, cartComm);

  if (rank == 0) {
    constructImage(imageHeight, imageWidth, img, image, loadBalanced, size);
  }

  MPI_Comm_free(&cartComm);
  free(localPixels);
  if (rank == 0) {
    free(img);
  }
}
