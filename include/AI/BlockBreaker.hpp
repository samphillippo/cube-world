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
    BlockBreaker(glm::vec3 center, float sideLength, std::shared_ptr<CubeMap> cubeMap, std::shared_ptr<Player> player);
    // Destructor
    ~BlockBreaker() override;
private:
    // Plans a path for the sentient cube
    void PlanPath() override;
    // overrides the super function to delete instead of add blocks
    Cube* Move() override;
    // Makes a move while in combat
    Cube* CombatMove() override;
};

#endif
