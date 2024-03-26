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
private:
    // Load jpg image to sample from
    void LoadImage(std::string filepath);
};

#endif
