#ifndef IMAGE_RENDERER_HPP_
#define IMAGE_RENDERER_HPP_

#include <fstream>
#include <string>
#include <vector>

#include "vec3.hpp"
#include "interval.hpp"

void toPpmFile(std::vector<std::vector<Pixel>> image, std::string file_path) {
    std::ofstream fout(file_path);
    if (fout.fail()) {
        return;
    }

    fout << "P3\n" << image[0].size() << " " << image.size() << "\n255\n";

    for (auto row : image) {
        for (auto pixel : row) {
            static const Interval intensity(0.000, 0.999);
            int rbyte = int(256 * intensity.clamp(pixel.x));
            int gbyte = int(256 * intensity.clamp(pixel.y));
            int bbyte = int(256 * intensity.clamp(pixel.z));
            fout << rbyte << " " << gbyte << " " << bbyte << "\n";
        }
    }
}

#endif