#include "AI/PathPlacer.hpp"

PathPlacer::PathPlacer(glm::vec3 center, float sideLength) : SentientCube(center, sideLength) {
    m_health = 5;
}

PathPlacer::~PathPlacer() {
}

void PathPlacer::OnTick() {
    m_tickCount++;
    m_damageTickCount = std::max(0, m_damageTickCount - 1);
}

bool PathPlacer::OnHit() {
    m_health--;
    if (m_health <= 0) {
        return true;
    }
    return false;
}
