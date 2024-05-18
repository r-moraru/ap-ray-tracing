#include <vector>

#include "image_renderer.hpp"
#include "viewport.hpp"

using namespace std;

int main() {
    double aspectRatio = 19.0 / 16.0;
    int imageWidth = 400;

    Viewport viewport(aspectRatio, imageWidth);

    vector<vector<Pixel>> image(viewport.imageHeight, vector<Pixel>(viewport.imageWidth));

    for (int i = 0; i < viewport.imageHeight; i++) {
        for (int j = 0; j < viewport.imageWidth; j++) {
            // cout << i << " " << j << endl;
            Vec3 pixelCenter = viewport.pixel00Center +
                    (i * viewport.pixelDeltaV) + (j * viewport.pixelDeltaU);
            Vec3 rayDirection = pixelCenter - viewport.cameraCenter;
            Ray r(viewport.cameraCenter, rayDirection);
            Pixel pixel = r.getColor();

            // cout << pixel << endl;
            image[i][j] = pixel;
        }
    }

    toPpmFile(image, "test.ppm");
}