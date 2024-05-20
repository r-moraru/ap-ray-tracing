#include <vector>

#include "hittable.hpp"
#include "hittable_list.hpp"
#include "image_renderer.hpp"
#include "lambertian.hpp"
#include "material.hpp"
#include "metal.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "strat_grid.hpp"
#include "strat_linear.hpp"
#include "utils.hpp"
#include "vec3.hpp"
#include "viewport.hpp"
#include <mpi.h>

using namespace std;

int main(int argc, char **argv) {
  double aspectRatio = 16.0 / 9.0;
  int imageWidth = 900;

  Viewport viewport(aspectRatio, imageWidth);

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

  // renderLinear(viewport, world, image);

  Strategy strategy = HORIZONTAL;
  bool loadBalanced = false;
  renderGrid(viewport, world, image, strategy, loadBalanced);

  MPI_Finalize();
  return 0;
}
