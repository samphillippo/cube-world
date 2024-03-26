/** @file PerlinNoise.hpp
 *  @brief PerlinNoise is a class used to programmatically generate noise, specifically z values given x,y values
 *
 * 	Uses a preloaded image to sample from and generate noise.
 *
 *  @author Sam
 *  @bug No known bugs.
 */
#include <string>
#ifndef PERLINNOISE_HPP
#define PERLINNOISE_HPP

class PerlinNoise {
public:
    // Constructor
    PerlinNoise();
    // Destructor
    ~PerlinNoise();
    // Get the noise value at a given x,y
    inline unsigned int GetNoiseValue(int x, int y) { return m_noiseMapPtr[x][y]; } //TODO: handle out of bounds
    // Get the width of the noise map
    inline unsigned int GetWidth() { return m_width; }
    // Get the height of the noise map
    inline unsigned int GetHeight() { return m_height; }
private:
    // Load jpg image to sample from
    void LoadImage(std::string filepath);
    unsigned int** m_noiseMapPtr;
    unsigned int m_width;
    unsigned int m_height;
};

#endif
