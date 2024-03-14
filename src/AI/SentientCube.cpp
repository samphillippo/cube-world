#include "AI/SentientCube.hpp"

SentientCube::SentientCube(glm::vec3 center, float sideLength) : Cube(center, sideLength) {
    m_tickCount = 0;
    m_damageTickCount = 0;
}

SentientCube::~SentientCube() {
}
