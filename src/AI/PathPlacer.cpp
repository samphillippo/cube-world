#include "AI/PathPlacer.hpp"
#include <iostream>

PathPlacer::PathPlacer(glm::vec3 center, float sideLength, std::shared_ptr<CubeMap> cubeMap,
                        std::shared_ptr<Player> player) : SentientCube(center, sideLength, cubeMap, player) {
    m_health = 5;
    m_movementTicks = 30;
    m_minActionTicks = 40;
    m_avgActionTicks = 40;
    m_combatMovementTicks = 10; //Triple speed while in combat
    m_maxPathRange = 8;
    m_minPathRange = 0;
    m_combatMovements = 0;
}

PathPlacer::~PathPlacer() {
}

glm::vec3 PathPlacer::GetRandomVector() {
    int x = (m_minPathRange + (rand() % (m_maxPathRange - m_minPathRange))) * (rand() % 2 ? 1 : -1);
    int y = (m_minPathRange + (rand() % (m_maxPathRange - m_minPathRange))) * (rand() % 2 ? 1 : -1) / 2;
    int z = (m_minPathRange + (rand() % (m_maxPathRange - m_minPathRange))) * (rand() % 2 ? 1 : -1);
    return glm::vec3(x, y, z);
}

//Pathfinds, following optimal route along randomly chosen vector
void PathPlacer::PlanPath() {
    m_path = PathToTarget(GetRandomVector());
    m_tickCount = m_movementTicks - 1;
}

Cube* PathPlacer::CombatMove() {
    if (m_combatMovements < 3) { //Move 3 times per planned path
        m_combatMovements++;
    } else {
        m_combatMovements = 0;
        m_path.clear();
        glm::vec3 pathVector = m_center - m_player->GetPosition();
        pathVector *= 10;
        m_path = PathToTarget(pathVector);
    }
    return Move();
}
