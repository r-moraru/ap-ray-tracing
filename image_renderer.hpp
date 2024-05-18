#ifndef IMAGE_RENDERER_HPP_
#define IMAGE_RENDERER_HPP_

#include <fstream>
#include <string>
#include <vector>

#include "vec3.hpp"

#define float_color_to_int(color) (int(255.9*color))

void toPpmFile(std::vector<std::vector<Pixel>> image, std::string file_path) {
    std::ofstream fout(file_path);
    if (fout.fail()) {
        return;
    }

    fout << "P3\n" << image[0].size() << " " << image.size() << "\n255\n";

    for (auto row : image) {
        for (auto pixel : row) {
            fout << float_color_to_int(pixel.x) << " " << float_color_to_int(pixel.y)
                    << " " << float_color_to_int(pixel.z) << "\n";
        }
    }
}

#endif