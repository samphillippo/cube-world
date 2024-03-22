#include "AI/BrickBuilder.hpp"
#include <iostream>

BrickBuilder::BrickBuilder(glm::vec3 center, float sideLength) : SentientCube(center, sideLength) {
    m_health = 5;
    m_movementTicks = 30;
    m_minActionTicks = 40;
    m_avgActionTicks = 40;
    m_maxPathRange = 8;
    m_minPathRange = 1;
    m_prevOppositeDirection = glm::vec3(0, 0, 0);
    m_maxLayers = 2;
    m_buildUpwards = true;
}

BrickBuilder::~BrickBuilder() {
}

//pick one of 3 directions, number of layers, and a random distance
//TODO: needs a sort of buffer, move once before starting calcs
void BrickBuilder::PlanPath(CubeMap& cubeMap) {
    m_path.clear();
    std::vector<glm::vec3> directions = { glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 0, -1) };
    for (int i = 0; i < 4; i++) {
        if (m_prevOppositeDirection == directions[i]) {
            directions.erase(directions.begin() + i);
            break;
        }
    }
    glm::vec3 direction = directions[rand() % directions.size()];
    m_prevOppositeDirection = direction * glm::vec3(-1, -1, -1);
    int distance = m_minPathRange + rand() % (m_maxPathRange - m_minPathRange);

    int layers = (rand() % m_maxLayers) + 1;
    //makes initial move
    glm::vec3 currentPosition = m_center;
    for (int i = 0; i < distance; i++) {
        currentPosition += direction;
        m_path.push_back(currentPosition);
    }
    currentPosition.y += m_buildUpwards ? 1 : -1;
    m_path.push_back(currentPosition);
    //repeats movements for each layer
    for (int i = 0; i < layers; i++) {
        for (int i = 0; i < distance; i++) {
            currentPosition -= direction;
            m_path.push_back(currentPosition);
        }
        currentPosition.y += m_buildUpwards ? 1 : -1;
        m_path.push_back(currentPosition);
        for (int i = 0; i < distance; i++) {
            currentPosition += direction;
            m_path.push_back(currentPosition);
        }
    }
    m_buildUpwards = !m_buildUpwards;
    m_tickCount = m_movementTicks - 1;
}

void BrickBuilder::OnTick(CubeMap& cubeMap) {
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

bool BrickBuilder::OnHit() {
    if (SentientCube::OnHit()) {
        return true;
    }
    //some behavior
    return false;
}
