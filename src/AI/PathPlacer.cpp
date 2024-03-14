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
    //TODO
}

void PathPlacer::Move() {
    //if next block is not open, plan instead

}

void PathPlacer::OnTick() {
    SentientCube::OnTick();
    if (m_isMoving) {
        Move();
    }
    if (m_tickCount >= m_minActionTicks) {
        if (rand() % m_avgActionTicks == 0) {
            m_isPlanning = true;
            m_isMoving = true;
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
