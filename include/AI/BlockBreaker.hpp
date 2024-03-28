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
    Cube* OnTick(CubeMap& cubeMap) override;
    // Called upon the sentient cube taking damage
    bool OnHit() override;
private:
    // Plans a path for the sentient cube
    void PlanPath(CubeMap& cubeMap) override;
    // overrides the super function to delete instead of add blocks
    Cube* Move(CubeMap& cubeMap) override;
};

#endif
