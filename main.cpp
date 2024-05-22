#include <mpi.h>
#include <string>
#include <vector>

#include "hittable_list.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "sphere.hpp"
#include "strat_linear.hpp"
#include "strat_parallel.hpp"
#include "utils.hpp"
#include "vec3.hpp"
#include "viewport.hpp"
#include "image_renderer.hpp"

using namespace std;

int main(int argc, char **argv) {
  int imageWidth = 1920, imageHeight = 1152;

  Viewport viewport(imageHeight, imageWidth);

  vector<vector<Pixel>> image(viewport.imageHeight,
                              vector<Pixel>(viewport.imageWidth));

  HittableList world;

  auto groundMaterial = make_shared<Lambertian>(Pixel({0.8, 0.8, 0.0}));
  auto centerMaterial = make_shared<Lambertian>(Pixel({0.1, 0.2, 0.5}));
  auto leftMaterial = make_shared<Metal>(Pixel({0.8, 0.8, 0.8}));
  auto rightMaterial = make_shared<Metal>(Pixel({0.8, 0.3, 0.2}));

  world.add(
      make_shared<Sphere>(Point({0.0, -100.5, -1.0}), 100.0, groundMaterial));
  world.add(make_shared<Sphere>(Point({0.0, 0.0, -1.5}), 0.5, centerMaterial));
  world.add(make_shared<Sphere>(Point({-1.0, 0.0, -1.0}), 0.5, leftMaterial));
  world.add(make_shared<Sphere>(Point({1.0, 0.0, -1.0}), 0.5, rightMaterial));

  MPI_Init(&argc, &argv);

  double start_time = MPI_Wtime();

  Topology topology = GRID;
  bool loadBalanced = true;

  if (argc > 1) {
    string arg = argv[1];
    if (arg == "grid") {
      topology = GRID;
      loadBalanced = true;
    } else if (arg == "grid_nlb") {
      topology = GRID;
      loadBalanced = false;
    } else if (arg == "ring") {
      topology = RING;
      loadBalanced = true;
    } else if (arg == "ring_nlb") {
      topology = RING;
      loadBalanced = false;
    } else if (arg == "linear") {
      topology = LINEAR;
    }
  }

  if (topology == LINEAR) {
    renderLinear(viewport, world, image);
  } else {
    renderGrid(viewport, world, image, topology, loadBalanced);
  }

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    if (!(argc > 2)) {
      toPpmFile(image, "test.ppm");
    }
  }

  double end_time = MPI_Wtime();
  if (rank == 0) {
    printf("Elapsed time: %f seconds\n", end_time - start_time);
  }

  MPI_Finalize();
  return 0;
}
