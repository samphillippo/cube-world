#include "AI/BrickBuilder.hpp"
#include <iostream>

BrickBuilder::BrickBuilder(glm::vec3 center, float sideLength, std::shared_ptr<CubeMap> cubeMap, std::shared_ptr<Player> player, std::shared_ptr<PerlinNoise> noiseMap) : SentientCube(center, sideLength, cubeMap, player) {
    m_health = 5;
    m_movementTicks = 10;
    m_minActionTicks = 40;
    m_avgActionTicks = 40;
    m_maxPathRange = 8;
    m_minPathRange = 1;
    m_prevOppositeDirection = glm::vec3(0, 0, 0);
    m_maxLayers = 1;
    m_buildUpwards = true;
    m_noiseMap = noiseMap;
}

BrickBuilder::~BrickBuilder() {
}

//pick one of 3 directions, number of layers, and a random distance
void BrickBuilder::PlanPath() {
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
    currentPosition += direction;
    m_path.push_back(currentPosition);

    //adjust height based on noise map
    //TODO: need to do this for every movement!!!!
    int targetY = m_noiseMap->GetNoiseValue(currentPosition.x, currentPosition.z) + 1;
    if (!m_buildUpwards) {
        targetY += 2;
    }
    while (currentPosition.y < targetY) {
        currentPosition.y += 1;
        m_path.push_back(currentPosition);
    }
    while (currentPosition.y > targetY) {
        currentPosition.y -= 1;
        m_path.push_back(currentPosition);
    }

    //moves first layer
    for (int i = 0; i < distance; i++) {
        currentPosition += direction;
        m_path.push_back(currentPosition);
    }

    //repeats movements for each additional layer
    for (int i = 0; i < layers; i++) {
        currentPosition.y += m_buildUpwards ? 1 : -1;
        m_path.push_back(currentPosition);
        for (int j = 0; j < distance; j++) {
            currentPosition -= direction;
            m_path.push_back(currentPosition);
        }
        currentPosition.y += m_buildUpwards ? 1 : -1;
        m_path.push_back(currentPosition);
        for (int j = 0; j < distance; j++) {
            currentPosition += direction;
            m_path.push_back(currentPosition);
        }
    }
    m_buildUpwards = !m_buildUpwards;
    m_tickCount = m_movementTicks - 1;
}

Cube* BrickBuilder::CombatMove() {
    return Move();
}
