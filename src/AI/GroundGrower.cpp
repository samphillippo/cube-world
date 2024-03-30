#include "AI/GroundGrower.hpp"
#include <iostream>

int GroundGrower::m_numPathingGrowers = 0;

GroundGrower::GroundGrower(glm::vec3 center, float sideLength,
                            std::shared_ptr<CubeMap> cubeMap, std::shared_ptr<Player> player,
                            std::shared_ptr<PerlinNoise> noiseMap, glm::vec3 buildDir, int initialPathLength) :
                            SentientCube(center, sideLength, cubeMap, player) {
    m_health = 15;
    m_movementTicks = 5;
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
void GroundGrower::PlanPath() {
    IncrementPathingGrowers();
    m_path.clear();
    glm::vec3 currentPos = m_center;
    for (int i = 0; i < m_pathLength; i++) {
        currentPos += m_buildDir;
        float goalY = m_noiseMap->GetNoiseValue(currentPos.x, currentPos.z);
        if (currentPos.y < goalY) { //if we want to go up, move over then go up
            m_path.push_back(currentPos);
            while (currentPos.y < goalY) {
                currentPos.y += 1;
                m_path.push_back(currentPos);
            }
        } else if (currentPos.y > goalY) { //if we want to go down, go down then move over
            currentPos -= m_buildDir;
            while (currentPos.y > goalY) {
                currentPos.y -= 1;
                m_path.push_back(currentPos);
            }
            currentPos += m_buildDir;
            m_path.push_back(currentPos);
        } else { //if we want to stay the same, just move over
            m_path.push_back(currentPos);
        }

    }
    m_path.push_back(currentPos + m_expandDir);
    m_buildDir *= -1;
    m_pathLength += 2;
    m_tickCount = m_movementTicks - 1;
}

//its somehow getting stuck in the pathing state
Cube* GroundGrower::OnTick() {
    SentientCube::OnTick();
    if (m_isMoving) { //only move on move ticks
        if (m_tickCount % m_movementTicks == 0) {
            m_tickCount = 0;
            if (m_path.size() == 0) {
                DecrementPathingGrowers();
            }
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
    else if (m_numPathingGrowers != 0) { //wait for all groundgrowers to finish pathing
        m_tickCount = 0;
    }
    return nullptr;
}

bool GroundGrower::OnHit(bool playerHit) {
    if (SentientCube::OnHit(playerHit)) {
        if (m_path.size() != 0) {
            //handles case where cube is destroyed while pathing
            DecrementPathingGrowers();
        }
        return true;
    }
    //NOTE: GroundGrowers currently ignore player attacks
    return false;
}
