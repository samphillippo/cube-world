#include "AI/SentientCube.hpp"

SentientCube::SentientCube(glm::vec3 center, float sideLength) : Cube(center, sideLength) {
    m_tickCount = 0;
    m_damageTickCount = 0;
    m_damageMaxTicks = 10;
    m_damageColor = glm::vec3(0.8f, 0.0f, 0.0f);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

SentientCube::~SentientCube() {
}

void SentientCube::OnTick(CubeMap& cubeMap) {
    m_tickCount++;
    m_damageTickCount = std::max(0, m_damageTickCount - 1);
    this->m_colorAdjustment = m_damageColor * (m_damageTickCount / (float)m_damageMaxTicks);
}

bool SentientCube::OnHit() {
    m_health--;
    m_damageTickCount = 10;
    if (m_health <= 0) {
        return true;
    }
    return false;
}

void SentientCube::Move(CubeMap& cubeMap) {
    //path is complete, stop moving restart cycle
    if (m_path.size() == 0) {
        m_isMoving = false;
        return;
    }
    //creates new cube at previous location
    Cube* newCube = new Cube(m_center, m_sideLength);
    newCube->SetTexture(m_textureDiffuse);
    cubeMap.RemoveCube(this);
    cubeMap.AddCube(newCube);
    //if there is a cube at the next location, remove it
    Cube* nextCube = cubeMap.GetCube(m_path[0].x, m_path[0].y, m_path[0].z);
    if (nextCube != nullptr) {
        cubeMap.RemoveCube(nextCube);
        delete nextCube;
    }
    m_center = m_path[0];
    this->Clear();
    this->Update();
    cubeMap.AddCube(this);
    m_path.erase(m_path.begin());
}

std::vector<glm::vec3> SentientCube::PathToTarget(glm::vec3 target) {
    std::vector<glm::vec3> path;
    glm::vec3 delta = abs(target);
    int sx = (0 <= target.x) ? 1 : -1;
    int sy = (0 <= target.y) ? 1 : -1;
    int sz = (0 <= target.z) ? 1 : -1;

    int numSteps = delta.x + delta.y + delta.z;
    glm::vec3 slope = glm::vec3(delta.x/ numSteps, delta.y/ numSteps, delta.z/ numSteps);
    glm::vec3 stepTracker = slope;

    glm::vec3 currentLocation = m_center;
    glm::vec3 finalLocation = m_center + target;

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

        path.push_back(currentLocation);
    }
    return path;
}
