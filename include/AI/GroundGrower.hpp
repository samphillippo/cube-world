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
    GroundGrower(glm::vec3 center, float sideLength, std::shared_ptr<CubeMap> cubeMap, std::shared_ptr<Player> player, std::shared_ptr<PerlinNoise> noiseMap, glm::vec3 buildDir, int initialPathLength);
    // Destructor
    ~GroundGrower() override;
private:
    // Plans a path for the sentient cube
    void PlanPath() override;
    // Makes a move while in combat
    Cube* CombatMove() override;
    // The noise map used to generate the ground
    std::shared_ptr<PerlinNoise> m_noiseMap;
    // The direction to build a strip of ground
    glm::vec3 m_buildDir;
    // The direction to move after building a strip
    glm::vec3 m_expandDir;
    // The length of path to build
    int m_pathLength;
};

#endif
