#include "Util/PerlinNoise.hpp"
#include "Util/Image.hpp"
#include <iostream>

PerlinNoise::PerlinNoise() {
    LoadImage("common/textures/perlin_noise.jpg");
    m_minHeight = -25;
    m_maxHeight = 15;
}

PerlinNoise::~PerlinNoise() {
    for (int i = 0; i < m_imageWidth; ++i) {
        delete[] m_noiseMapPtr[i];
    }
    delete[] m_noiseMapPtr;
}

void PerlinNoise::LoadImage(std::string filepath) {
    Image image(filepath);
    image.LoadJPG();
    m_imageWidth = image.GetWidth();
    m_imageHeight = image.GetHeight();
    m_imageCenterX = m_imageWidth / 2;
    m_imageCenterY = m_imageHeight / 2;

    //copies data from Image to m_noiseMapPtr
    m_noiseMapPtr = new unsigned int*[m_imageWidth];
    for (int i = 0; i < m_imageWidth; i++) {
        m_noiseMapPtr[i] = new unsigned int[m_imageHeight];
        for (int j = 0; j < m_imageHeight; j++) {
            m_noiseMapPtr[i][j] = image.GetPixelR(i, j);
        }
    }
}

int PerlinNoise::GetNoiseValue(int x, int y) {
    //TODO: will get sharp edges at bounds of image
    x = (x + m_imageCenterX) % m_imageWidth;
    y = (y + m_imageCenterY) % m_imageHeight;
    unsigned int rawNoiseValue = m_noiseMapPtr[x][y];
    return (int)(rawNoiseValue / 256.0f * (m_maxHeight - m_minHeight) + m_minHeight);
}
