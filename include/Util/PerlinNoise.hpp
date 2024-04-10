/** @file PerlinNoise.hpp
 *  @brief PerlinNoise is a class used to programmatically generate noise, specifically z values given x,y values
 *
 * 	Uses a preloaded image to sample from and generate noise.
 *
 *  @author Sam
 *  @bug No known bugs.
 */
#include <string>
#include <glm/glm.hpp>
#ifndef PERLINNOISE_HPP
#define PERLINNOISE_HPP

class PerlinNoise {
public:
    // Constructor
    PerlinNoise();
    // Destructor
    ~PerlinNoise();
    // Get the noise value at a given x,y
    int GetNoiseValue(int x, int y);
    // Get the width of the noise map
    inline unsigned int GetWidth() { return m_imageWidth; }
    // Get the height of the noise map
    inline unsigned int GetHeight() { return m_imageHeight; }
    // Returns the positions of the initial cubes to exist upon world creation
    std::vector<glm::vec3> GetInitialGroundCubes(int initialAreaSize);
private:
    // Load jpg image to sample from
    void LoadImage(std::string filepath);
    //image information
    unsigned int** m_noiseMapPtr;
    unsigned int m_imageWidth;
    unsigned int m_imageHeight;
    unsigned int m_imageCenterX, m_imageCenterY;
    //min and max height to return
    int m_minHeight;
    int m_maxHeight;
    //initial random offsets
    int m_xOffset;
    int m_yOffset;
};

#endif
