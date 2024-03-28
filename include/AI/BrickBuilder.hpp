/** @file BrickBuilder.hpp
 *  @brief BrickBuilder is a sentient cube that creates brick walls around the environment.
 *
 * 	Contains necessary methods to move and respond to damage.
 *
 *  @author Sam
 *  @bug No known bugs.
 */
#ifndef BRICKBUILDER_HPP
#define BRICKBUILDER_HPP

#include "AI/SentientCube.hpp"
#include "Util/PerlinNoise.hpp"

class BrickBuilder: public SentientCube {
public:
    // Constructor
    BrickBuilder(glm::vec3 center, float sideLength, std::shared_ptr<PerlinNoise> noiseMap);
    // Destructor
    ~BrickBuilder() override;
    // Updates the sentient cube's state each tick
    Cube* OnTick(CubeMap& cubeMap) override;
    // Called upon the sentient cube taking damage
    bool OnHit() override;
private:
    // Plans a path for the sentient cube
    void PlanPath(CubeMap& cubeMap) override;
    //max range of path placement
    int m_maxPathRange;
    //min range of path placement
    int m_minPathRange;
    //vector pointing opposite to the previous direction of travel
    glm::vec3 m_prevOppositeDirection;
    //max number of layers to build
    int m_maxLayers;
    //should the brickbuilder build its walls from bottom or top
    bool m_buildUpwards;
    // The noise map used to know where wall base should be
    std::shared_ptr<PerlinNoise> m_noiseMap;
};

#endif
