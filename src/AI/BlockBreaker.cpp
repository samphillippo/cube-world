#include "AI/BlockBreaker.hpp"
#include <iostream>
#include <queue>
#include <set>
#include <random>

BlockBreaker::BlockBreaker(glm::vec3 center, float sideLength, std::shared_ptr<CubeMap> cubeMap, std::shared_ptr<Player> player) : SentientCube(center, sideLength, cubeMap, player) {
    m_health = 5;
    m_movementTicks = 20;
    m_minActionTicks = 40;
    m_avgActionTicks = 1;
}

BlockBreaker::~BlockBreaker() {

}

//gets all neighbors to given coordinates
std::vector<Coordinates> GetNeighbors(Coordinates coords) {
    std::vector<Coordinates> neighbors;
    neighbors.push_back({coords.x + 1, coords.y, coords.z});
    neighbors.push_back({coords.x - 1, coords.y, coords.z});
    neighbors.push_back({coords.x, coords.y + 1, coords.z});
    neighbors.push_back({coords.x, coords.y - 1, coords.z});
    neighbors.push_back({coords.x, coords.y, coords.z + 1});
    neighbors.push_back({coords.x, coords.y, coords.z - 1});
    std::shuffle(neighbors.begin(), neighbors.end(), std::default_random_engine(std::rand()));
    return neighbors;

}

//Pathfinds, BFSing to nearest cube(s)
void BlockBreaker::PlanPath() {
    m_path.clear();

    std::queue<Coordinates> q;
    std::set<Coordinates> visited;
    Coordinates startCoords = {(int)m_center.x, (int)m_center.y, (int)m_center.z};
    visited.insert(startCoords);
    for (Coordinates neighbor : GetNeighbors(startCoords)) {
        if (visited.find(neighbor) == visited.end()) {
            q.push(neighbor);
        }
    }

    int count = 0;
    while (!q.empty()) {
        Coordinates currentCube = q.front();
        q.pop();

        visited.insert(currentCube);
        if (m_cubeMap->GetCube(currentCube.x, currentCube.y, currentCube.z) != nullptr) {
            m_path = PathToTarget(glm::vec3(currentCube.x - m_center.x, currentCube.y - m_center.y, currentCube.z - m_center.z));
            break;
        }
        for (Coordinates neighbor : GetNeighbors(currentCube)) {
            if (visited.find(neighbor) == visited.end()) {
                q.push(neighbor);
            }
        }
        count ++;
        if (count > 5000) { //if no cube has been found within the first 5000 checks give up
            break;
        }
    }

    m_tickCount = m_movementTicks - 1;
}

Cube* BlockBreaker::Move() {
    if (m_path.size() == 0) {
        m_isMoving = false;
        return nullptr;
    }
    Cube* deletedCube = nullptr;
    //if the next block is occupied
    Cube* nextCube = m_cubeMap->GetCube(m_path[0].x, m_path[0].y, m_path[0].z);
    if (nextCube != nullptr) {
        if (nextCube->OnHit(false)) {
            m_cubeMap->RemoveCube(nextCube);
            deletedCube = nextCube;
            delete nextCube;
        } else {
            return nullptr; //if the cube is not destroyed, stop repeat movement next cycle
        }
    }
    m_cubeMap->RemoveCube(this);
    m_center = m_path[0];
    this->Clear();
    this->Update();
    m_cubeMap->AddCube(this);
    m_path.erase(m_path.begin());
    return deletedCube;
}

Cube* BlockBreaker::CombatMove() {
    return Move();
}
