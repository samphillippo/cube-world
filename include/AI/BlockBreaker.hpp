/** @file BlockBreaker.hpp
 *  @brief BlockBreaker is a sentient cube that moves around destroying blocks.
 *
 * 	Contains necessary methods to move and respond to damage.
 *
 *  @author Sam
 *  @bug No known bugs.
 */
#ifndef BLOCKBREAKER_HPP
#define BLOCKBREAKER_HPP

#include "AI/SentientCube.hpp"

class BlockBreaker: public SentientCube {
public:
    // Constructor
    BlockBreaker(glm::vec3 center, float sideLength);
    // Destructor
    ~BlockBreaker() override;
    // Updates the sentient cube's state each tick
    void OnTick(CubeMap& cubeMap) override;
    // Called upon the sentient cube taking damage
    bool OnHit() override;
    // Plans a path for the sentient cube
    void PlanPath() override;
private:
    // // Gets a random vector in range
    // glm::vec3 GetRandomVector();
    // //max range of path placement
    // int m_maxPathRange;
    // //min range of path placement
    // int m_minPathRange;
};

#endif
