#include "AI/GroundGrower.hpp"
#include <iostream>

GroundGrower::GroundGrower(glm::vec3 center, float sideLength, int initialAreaX, int initialAreaZ) : SentientCube(center, sideLength) {
    m_health = 5;
    m_movementTicks = 10;
    m_minActionTicks = 40;
    m_avgActionTicks = 40;
    m_initialAreaX = initialAreaX;
    m_initialAreaZ = initialAreaZ;
}

GroundGrower::~GroundGrower() {
}

//start from center, do loops around...
void GroundGrower::PlanPath(CubeMap& cubeMap) {
    m_path.clear();
    // for (int i = 0; i < 20; i++) {
    //     for (int j = 0; j < 20; j++) {
    //         std::cout << m_noiseMap.GetNoiseValue(i - 10, j - 10) << " ";
    //     }
    //     std::cout << std::endl;
    // }
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

std::vector<glm::vec3> GroundGrower::GetInitialGroundCubes() {
    std::vector<glm::vec3> groundCubes;
    int startX = m_initialAreaX / 2 - m_initialAreaX;
    int endX = m_initialAreaX / 2;
    int startZ = m_initialAreaZ / 2 - m_initialAreaZ;
    int endZ = m_initialAreaZ / 2;
    for (int i = startX; i < endX; i++) {
        for (int j = startZ; j < endZ; j++) {
            groundCubes.push_back(glm::vec3(i, m_noiseMap.GetNoiseValue(i, j), j));
        }
    }
    return groundCubes;
}
