#include "AI/PathPlacer.hpp"
#include <iostream>
#include <cstdlib>

PathPlacer::PathPlacer(glm::vec3 center, float sideLength) : SentientCube(center, sideLength) {
    m_health = 5;
    m_movementTicks = 10;
    m_minActionTicks = 20;
    m_avgActionTicks = 40;
}

PathPlacer::~PathPlacer() {
}

void PathPlacer::PlanPath() {
    std::cout << "PathPlacer PlanPath" << std::endl;
    //TODO
    m_path.clear();
    m_path.push_back(glm::vec3(2, 2, 2));
    //forces next tick to be a move tick
    m_tickCount = m_movementTicks - 1;
}

void PathPlacer::Move(CubeMap& cubeMap) {
    std::cout << "PathPlacer Move" << std::endl;
    //path is complete, stop moving restart cycle
    if (m_path.size() == 0) {
        m_isMoving = false;
        return;
    }
    // glm::vec3 nextCoords = m_path[0];
    // std::cout << "Next coords: " << nextCoords.x << " " << nextCoords.y << " " << nextCoords.z << std::endl;
    // Cube* nextCube = cubeMap.GetCube(nextCoords.x, nextCoords.y, nextCoords.z);
    // std::cout << "Next cube: " << nextCube << std::endl;
    // return;

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
        glm::vec3 nextCoords = m_path[0];
        std::cout << "Next coords: " << nextCoords.x << " " << nextCoords.y << " " << nextCoords.z << std::endl;
        std::cout << "Current coords: " << m_center.x << " " << m_center.y << " " << m_center.z << std::endl;
        //if the next block is open, move there
        m_isPlanning = false;
        cubeMap.RemoveCube(this);
        m_center = nextCoords;
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
