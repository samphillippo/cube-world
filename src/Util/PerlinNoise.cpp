#include "Util/PerlinNoise.hpp"
// #include "Util/stb_image.h"
#include <iostream>

PerlinNoise::PerlinNoise() {
    LoadImage("common/perlin_noise.jpg");
}

PerlinNoise::~PerlinNoise() {
    // Destructor
}

void PerlinNoise::LoadImage(std::string filepath) {
    // Load in the image
    //std::cout << "Loading image..." << std::endl;
    // int width, height, nrChannels;
    // unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
    // if (data)
    // {
    //     std::cout << "Image loaded." << std::endl;
    // }
    // else
    // {
    //     std::cerr << "Failed to load image." << std::endl;
    // }
    // stbi_image_free(data);
}
