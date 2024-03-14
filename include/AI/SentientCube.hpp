/** @file SentientCube.hpp
 *  @brief SentientCube is a base class used to implement cubes capable of interacting with the environment.
 *
 * 	Contains necessary methods to... TODO
 *
 *  @author Sam
 *  @bug No known bugs.
 */
#ifndef SENTIENTCUBE_HPP
#define SENTIENTCUBE_HPP

#include "Cube.hpp"

class SentientCube: public Cube {
public:
    // Constructor
    SentientCube(glm::vec3 center, float sideLength);
    // Destructor
    ~SentientCube();
    // Updates the sentient cube's state each tick
    virtual void OnTick() = 0;
    // Called upon the sentient cube taking damage
    virtual void OnHit() = 0;
protected:
    int m_tickCount;
};

#endif
