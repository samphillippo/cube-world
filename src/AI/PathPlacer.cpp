#include "AI/PathPlacer.hpp"
#include <iostream>
#include <cstdlib>

PathPlacer::PathPlacer(glm::vec3 center, float sideLength) : SentientCube(center, sideLength) {
    m_health = 5;
    m_movementTicks = 30;
    m_minActionTicks = 40;
    m_avgActionTicks = 40;
    m_maxPathRange = 8;
    m_minPathRange = 1;
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
    glm::vec3 randomVector = GetRandomVector();
    m_path.clear();

    //Total distance needed to travel in each direction
    glm::vec3 delta = abs(randomVector);
    int sx = (0 <= randomVector.x) ? 1 : -1;
    int sy = (0 <= randomVector.y) ? 1 : -1;
    int sz = (0 <= randomVector.z) ? 1 : -1;

    int numSteps = delta.x + delta.y + delta.z;
    glm::vec3 slope = glm::vec3(delta.x/ numSteps, delta.y/ numSteps, delta.z/ numSteps);
    glm::vec3 stepTracker = slope;

    glm::vec3 currentLocation = m_center;
    glm::vec3 finalLocation = m_center + randomVector;

    for (int i = 0; i < numSteps; i++) {
        if (stepTracker.x >= stepTracker.y && stepTracker.x >= stepTracker.z) {
            currentLocation.x += sx;
            stepTracker.x -= 1;
        } else if (stepTracker.y >= stepTracker.z) {
            currentLocation.y += sy;
            stepTracker.y -= 1;
        } else {
            currentLocation.z += sz;
            stepTracker.z -= 1;
        }
        stepTracker += slope;

        m_path.push_back(currentLocation);
    }
    m_tickCount = m_movementTicks - 1;
}

void PathPlacer::Move(CubeMap& cubeMap) {
    //path is complete, stop moving restart cycle
    if (m_path.size() == 0) {
        m_isMoving = false;
        return;
    }

    //if the next block is occupied
    if (cubeMap.GetCube(m_path[0].x, m_path[0].y, m_path[0].z) != nullptr) {
        //if we're still planning, keep planning
        if (m_isPlanning) {
            PlanPath();
            return;
        } else {
            //if we're not planning, stop moving, restart cycle
            m_isMoving = false;
            return;
        }
    } else {
        //if the next block is open, move there
        m_isPlanning = false;
        cubeMap.RemoveCube(this);
        m_center = m_path[0];
        this->Clear();
        this->Update();
        cubeMap.AddCube(this);
        m_path.erase(m_path.begin());
    }

}

void PathPlacer::OnTick(CubeMap& cubeMap) {
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


//how do we decide when we want to move?

//Random amount of time (x-y) to decide to move
//When we've decided, try to generate a path every movement tick
    //when its stopped on first move, immediately generate a new path
    //when its stopped on later moves, reset decision to move

//When we DO want to move:
//1. choose random vector
//2. check first block along path
//3. if block is open, move there, continue following path until completion

//might need to take in player as input
//or should all sentient cubs store player?
bool PathPlacer::OnHit() {
    if (SentientCube::OnHit()) {
        return true;
    }
    //some behavior
    return false;
}
