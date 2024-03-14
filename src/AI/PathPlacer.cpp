#include "AI/PathPlacer.hpp"

PathPlacer::PathPlacer(glm::vec3 center, float sideLength) : SentientCube(center, sideLength) {
    //nothing needed here
}

PathPlacer::~PathPlacer() {
}

void PathPlacer::OnTick() {
    m_tickCount++;
    if (m_tickCount % 60 == 0) {
    }
}

bool PathPlacer::OnHit() {
    return false;
}
