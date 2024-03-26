#include "Util/PerlinNoise.hpp"
#include "Util/Image.hpp"
#include <iostream>

PerlinNoise::PerlinNoise() {
    LoadImage("common/textures/perlin_noise.jpg");
}

PerlinNoise::~PerlinNoise() {
    for (int i = 0; i < m_width; ++i) {
        delete[] m_noiseMapPtr[i];
    }
    delete[] m_noiseMapPtr;
}

void PerlinNoise::LoadImage(std::string filepath) {
    Image image(filepath);
    image.LoadJPG();
    m_width = image.GetWidth();
    m_height = image.GetHeight();
    m_noiseMapPtr = new unsigned int*[m_width];
    for (int i = 0; i < m_width; i++) {
        m_noiseMapPtr[i] = new unsigned int[m_height];
        for (int j = 0; j < m_height; j++) {
            m_noiseMapPtr[i][j] = image.GetPixelR(i, j);
        }
    }
}
