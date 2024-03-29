#include "AI/PathPlacer.hpp"
#include <iostream>

PathPlacer::PathPlacer(glm::vec3 center, float sideLength, std::shared_ptr<CubeMap> cubeMap) : SentientCube(center, sideLength, cubeMap) {
    m_health = 5;
    m_movementTicks = 30;
    m_minActionTicks = 40;
    m_avgActionTicks = 40;
    m_maxPathRange = 8;
    m_minPathRange = 0;
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

Cube* PathPlacer::OnTick() {
    SentientCube::OnTick();
    if (m_isMoving) { //only move on move ticks
        if (m_tickCount % m_movementTicks == 0) {
            m_tickCount = 0;
            return Move();
        }
    }
    else if (m_tickCount >= m_minActionTicks) {
        if (rand() % m_avgActionTicks == 0) {
            m_isMoving = true;
            PlanPath();
            m_tickCount = 0;
        }
    }
    return nullptr;
}

bool PathPlacer::OnHit() {
    if (SentientCube::OnHit()) {
        return true;
    }
    //some behavior
    return false;
}
