#include "AI/BlockBreaker.hpp"
#include <iostream>
#include <queue>
#include <set>
#include <random>

BlockBreaker::BlockBreaker(glm::vec3 center, float sideLength) : SentientCube(center, sideLength) {
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
void BlockBreaker::PlanPath(CubeMap& cubeMap) {
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
        if (cubeMap.GetCube(currentCube.x, currentCube.y, currentCube.z) != nullptr) {
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
            m_isMoving = true;
            PlanPath(cubeMap);
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
