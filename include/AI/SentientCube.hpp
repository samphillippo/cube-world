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

#include "Util/CubeMap.hpp"
#include "Player.hpp"
#include <cstdlib>
#include <ctime>

class SentientCube: public Cube {
public:
    // Constructor
    SentientCube(glm::vec3 center, float sideLength, std::shared_ptr<CubeMap> cubeMap, std::shared_ptr<Player> player);
    // Destructor
    virtual ~SentientCube();
    // Updates the sentient cube's state each tick
    Cube* OnTick() override;
    // Called upon the sentient cube taking damage
    bool OnHit(bool playerHit) override;
protected:
    // Plans a path for the sentient cube
    virtual void PlanPath() = 0;
    // Moves the sentient cube along its path
    virtual Cube* Move();
    // Function that takes in a vector and produces a path that follows it
    std::vector<glm::vec3> PathToTarget(glm::vec3 target);
    // Counts number of ticks since last action
    int m_tickCount;
    // Health of the sentient cube
    int m_health;
    // How many ticks between steps on path
    int m_movementTicks;
    // Minimum number of ticks between actions
    int m_minActionTicks;
    // Average number of ticks to wait between actions, after min
    int m_avgActionTicks;
    // Whether the sentient cube is moving
    bool m_isMoving;
    // Whether the sentient cube is in its "combat" state
    bool m_isInCombat;
    // Current planned path
    std::vector<glm::vec3> m_path;
    // Mapping of coordinates to all cubes in the scene
    std::shared_ptr<CubeMap> m_cubeMap;
    // A reference to the player
    std::shared_ptr<Player> m_player;

    //DAMAGE INFO
    // How many ticks of damage animation are left
    int m_damageTickCount;
    // Color adjustment for damage animation
    glm::vec3 m_damageColor;
    // Number of ticks for damage animation
    int m_damageMaxTicks;
    // Distance from player needed to return to default state
    float m_escapeDistance;
};

#endif
