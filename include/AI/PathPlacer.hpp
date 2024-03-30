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
    PathPlacer(glm::vec3 center, float sideLength, std::shared_ptr<CubeMap> cubeMa, std::shared_ptr<Player> player);
    // Destructor
    ~PathPlacer() override;
    // Updates the sentient cube's state each tick
    Cube* OnTick() override;
private:
    // Plans a path for the sentient cube
    void PlanPath() override;
    // Gets a random vector in range
    glm::vec3 GetRandomVector();
    //max range of path placement
    int m_maxPathRange;
    //min range of path placement
    int m_minPathRange;
};

#endif
