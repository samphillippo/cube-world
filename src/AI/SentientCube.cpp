#include "AI/SentientCube.hpp"

SentientCube::SentientCube(glm::vec3 center, float sideLength) : Cube(center, sideLength) {
    m_tickCount = 0;
    m_damageTickCount = 0;
    m_damageMaxTicks = 10;
    m_damageColor = glm::vec3(0.8f, 0.0f, 0.0f);
}

SentientCube::~SentientCube() {
}

bool SentientCube::OnHit() {
    m_health--;
    m_damageTickCount = 10;
    if (m_health <= 0) {
        return true;
    }
    return false;
}
