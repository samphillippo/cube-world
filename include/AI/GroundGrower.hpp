/** @file GroundGrower.hpp
 *  @brief GroundGrower is a sentient cube that builds a grass flooring for the environment
 *
 * 	Contains necessary methods to move and respond to damage.
 *
 *  @author Sam
 *  @bug No known bugs.
 */
#ifndef GROUNDGROWER_HPP
#define GROUNDGROWER_HPP

#include "AI/SentientCube.hpp"
#include "Util/PerlinNoise.hpp"

class GroundGrower: public SentientCube {
public:
    // Constructor
    GroundGrower(glm::vec3 center, float sideLength, int initialAreaX, int initialAreaZ);
    // Destructor
    ~GroundGrower() override;
    // Updates the sentient cube's state each tick
    void OnTick(CubeMap& cubeMap) override;
    // Called upon the sentient cube taking damage
    bool OnHit() override;
    // Returns the initial cubes to exist upon world creation
    std::vector<glm::vec3> GetInitialGroundCubes();
private:
    // Plans a path for the sentient cube
    void PlanPath(CubeMap& cubeMap) override;
    // The noise map used to generate the ground
    PerlinNoise m_noiseMap;
    // initial X width of the area to generate ground
    int m_initialAreaX;
    // initial Z width of the area to generate ground
    int m_initialAreaZ;
};

#endif
