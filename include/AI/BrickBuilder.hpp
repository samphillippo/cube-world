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

class BrickBuilder: public SentientCube {
public:
    // Constructor
    BrickBuilder(glm::vec3 center, float sideLength);
    // Destructor
    ~BrickBuilder() override;
    // Updates the sentient cube's state each tick
    void OnTick(CubeMap& cubeMap) override;
    // Called upon the sentient cube taking damage
    bool OnHit() override;
private:
    // Plans a path for the sentient cube
    void PlanPath(CubeMap& cubeMap) override;
    // Gets a random vector in range
    glm::vec3 GetRandomVector();
    //max range of path placement
    int m_maxPathRange;
    //min range of path placement
    int m_minPathRange;
};

#endif
