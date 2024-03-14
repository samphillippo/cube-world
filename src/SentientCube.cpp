#include "SentientCube.hpp"

SentientCube::SentientCube(glm::vec3 center, float sideLength) : Cube(center, sideLength) {
    m_tickCount = 0;
}

SentientCube::~SentientCube() {
}

void SentientCube::OnTick() {
    m_tickCount++;
}
