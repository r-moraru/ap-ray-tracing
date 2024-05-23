#include <mpi.h>
#include <string>

#include "include/hittable_list.hpp"
#include "include/image_renderer.hpp"
#include "include/lambertian.hpp"
#include "include/metal.hpp"
#include "include/sphere.hpp"
#include "include/strat_linear.hpp"
#include "include/strat_parallel.hpp"
#include "include/utils.hpp"
#include "include/vec3.hpp"
#include "include/viewport.hpp"

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
  auto frontMaterial = make_shared<Metal>(Pixel({0.8, 0.8, 0.8}));
  auto upMaterial = make_shared<Metal>(Pixel({0.7, 0.7, 0.7}));

  world.add(
      make_shared<Sphere>(Point({0.0, -100.5, -1.0}), 100.0, groundMaterial));
  world.add(make_shared<Sphere>(Point({0.0, 0.0, -1.5}), 0.5, centerMaterial));
  world.add(make_shared<Sphere>(Point({-1.0, 0.0, -1.0}), 0.5, leftMaterial));
  world.add(make_shared<Sphere>(Point({1.0, 0.0, -1.0}), 0.5, rightMaterial));
  world.add(make_shared<Sphere>(Point({0.0, 1.0, 0}), 0.4, upMaterial));
  world.add(make_shared<Sphere>(Point({0.0, -0.25, 1}), 0.25, frontMaterial));
  world.add(make_shared<Sphere>(Point({0.0, -0.20, -0.5}), 0.3, frontMaterial));
  world.add(make_shared<Sphere>(Point({0.0, 0.60, -0.25}), 0.25, groundMaterial));

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

  int rank = 0;
  double start_time = MPI_Wtime();
  if (topology == LINEAR) {
    renderLinear(viewport, world, image);
  } else {
    MPI_Init(&argc, &argv);
    renderGrid(viewport, world, image, topology, loadBalanced);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Finalize();
  }

  double end_time = MPI_Wtime();
  if (rank == 0) {
    printf("Elapsed time: %f seconds\n", end_time - start_time);

    if (!(argc > 2)) {
      toPpmFile(image, "test.ppm");
    }
  }
  return 0;
}
