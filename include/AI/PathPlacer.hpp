/** @file PathPlacer.hpp
 *  @brief PathPlacer is a sentient cube that randomly places paths in the environment.
 *
 * 	Contains necessary methods to move and respond to damage.
 *
 *  @author Sam
 *  @bug No known bugs.
 */
#ifndef PATHPLACER_HPP
#define PATHPLACER_HPP

#include "AI/SentientCube.hpp"

class PathPlacer: public SentientCube {
public:
    // Constructor
    PathPlacer(glm::vec3 center, float sideLength);
    // Destructor
    ~PathPlacer() override;
    // Updates the sentient cube's state each tick
    void OnTick(CubeMap& cubeMap) override;
    // Called upon the sentient cube taking damage
    bool OnHit() override;
    // Plans a path for the sentient cube
    void PlanPath() override;
    // Moves the sentient cube along its path
    void Move(CubeMap& cubeMap) override;
private:
    //max range of path placement
    int m_maxPathRange;
    //min range of path placement
    int m_minPathRange;
};

#endif
