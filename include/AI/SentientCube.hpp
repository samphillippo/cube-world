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
    virtual ~SentientCube();
    // Updates the sentient cube's state each tick
    virtual void OnTick() = 0;
    // Called upon the sentient cube taking damage
    virtual bool OnHit();
protected:
    // Counts number of ticks since last action
    int m_tickCount;
    // Health of the sentient cube
    int m_health;
    // How many ticks of damage animation are left
    int m_damageTickCount;
    // Color adjustment for damage animation
    glm::vec3 m_damageColor;
    // Number of ticks for damage animation
    int m_damageMaxTicks;
};

#endif
