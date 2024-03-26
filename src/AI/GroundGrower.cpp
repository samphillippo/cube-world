#include "AI/GroundGrower.hpp"
#include <iostream>

GroundGrower::GroundGrower(glm::vec3 center, float sideLength, std::shared_ptr<PerlinNoise> noiseMap, glm::vec3 buildDir, int initialPathLength) : SentientCube(center, sideLength) {
    m_health = 5;
    m_movementTicks = 10;
    m_minActionTicks = 40;
    m_avgActionTicks = 1;
    m_noiseMap = noiseMap;
    m_buildDir = buildDir;
    m_expandDir = glm::vec3(-buildDir.z, 0, buildDir.x);
    m_pathLength = initialPathLength;
}

GroundGrower::~GroundGrower() {
}

//start from center, do loops around...
void GroundGrower::PlanPath(CubeMap& cubeMap) {
    m_path.clear();
    glm::vec3 currentPos = m_center;
    for (int i = 0; i < m_pathLength; i++) {
        currentPos += m_buildDir;
        m_path.push_back(currentPos);
    }
    m_path.push_back(currentPos + m_expandDir);
    m_buildDir *= -1;
    m_pathLength += 2;
    m_tickCount = m_movementTicks - 1;
}

void GroundGrower::OnTick(CubeMap& cubeMap) {
    SentientCube::OnTick(cubeMap);
    if (m_isMoving) { //only move on move ticks
        if (m_tickCount % m_movementTicks == 0) {
            m_tickCount = 0;
            Move(cubeMap);
        }
    }
    else if (m_tickCount >= m_minActionTicks) {
        if (rand() % m_avgActionTicks == 0) {
            m_isPlanning = true;
            m_isMoving = true;
            PlanPath(cubeMap);
            m_tickCount = 0;
        }
    }

}

bool GroundGrower::OnHit() {
    if (SentientCube::OnHit()) {
        return true;
    }
    //some behavior
    return false;
}
