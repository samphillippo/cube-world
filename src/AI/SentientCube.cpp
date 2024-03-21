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

    //if the next block is occupied
    if (cubeMap.GetCube(m_path[0].x, m_path[0].y, m_path[0].z) != nullptr) { //if we're still planning, keep planning
        if (m_isPlanning) {
            PlanPath();
            return;
        } else { //if we're not planning, stop moving, restart cycle
            m_isMoving = false;
            return;
        }
    } else { //if the next block is open, move there
        m_isPlanning = false;
        //creates new cube at previous location
        Cube* newCube = new Cube(m_center, m_sideLength);
        newCube->SetTexture(m_textureDiffuse);
        cubeMap.RemoveCube(this);
        m_center = m_path[0];
        this->Clear();
        this->Update();
        cubeMap.AddCube(this);
        cubeMap.AddCube(newCube);
        m_path.erase(m_path.begin());
    }
}
