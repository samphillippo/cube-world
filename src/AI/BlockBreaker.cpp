#include "AI/BlockBreaker.hpp"
#include <iostream>

BlockBreaker::BlockBreaker(glm::vec3 center, float sideLength) : SentientCube(center, sideLength) {
    m_health = 5;
    m_movementTicks = 30;
    m_minActionTicks = 40;
    m_avgActionTicks = 40;
}

BlockBreaker::~BlockBreaker() {

}

//Pathfinds, following optimal route along randomly chosen vector
void BlockBreaker::PlanPath() {
    m_path.clear();
    m_path.push_back(glm::vec3(1, 0, 1));
    m_path.push_back(glm::vec3(0, 0, 1));
    m_tickCount = m_movementTicks - 1;
}

void BlockBreaker::Move(CubeMap& cubeMap) {
    if (m_path.size() == 0) {
        m_isMoving = false;
        return;
    }

    //if the next block is occupied
    if (cubeMap.GetCube(m_path[0].x, m_path[0].y, m_path[0].z) != nullptr) {
        //remove the block at the next location
        cubeMap.RemoveCube(cubeMap.GetCube(m_path[0].x, m_path[0].y, m_path[0].z));
    }
    m_isPlanning = false;
    cubeMap.RemoveCube(this);
    m_center = m_path[0];
    this->Clear();
    this->Update();
    cubeMap.AddCube(this);
    m_path.erase(m_path.begin());
}

void BlockBreaker::OnTick(CubeMap& cubeMap) {
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
            PlanPath();
            m_tickCount = 0;
        }
    }

}

bool BlockBreaker::OnHit() {
    if (SentientCube::OnHit()) {
        return true;
    }
    //some behavior
    return false;
}
