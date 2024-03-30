#include "AI/SentientCube.hpp"

SentientCube::SentientCube(glm::vec3 center, float sideLength, std::shared_ptr<CubeMap> cubeMap, std::shared_ptr<Player> player) : Cube(center, sideLength) {
    m_cubeMap = cubeMap;
    m_player = player;
    m_tickCount = 0;
    m_damageTickCount = 0;
    m_escapeDistance = 20.0f; //how many blocks away is escape
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

SentientCube::~SentientCube() {
}

Cube* SentientCube::OnTick() {
    m_tickCount++;
    m_damageTickCount = std::max(0, m_damageTickCount - 1);
    this->m_colorAdjustment = m_damageColor * (m_damageTickCount / (float)m_damageMaxTicks);

    if (m_isInCombat) {
        if (glm::distance(m_center, m_player->GetPosition()) >= m_escapeDistance) {
            m_isInCombat = false;
        }
    }
    if (m_isMoving) { //only move on move ticks
        if (m_tickCount % m_movementTicks == 0) {
            m_tickCount = 0;
            return Move();
        }
    }
    else if (m_tickCount >= m_minActionTicks) {
        if (rand() % m_avgActionTicks == 0) {
            m_isMoving = true;
            PlanPath();
            m_tickCount = 0;
        }
    }
    return nullptr;
}

bool SentientCube::OnHit(bool playerHit) {
    m_health--;
    m_damageTickCount = 10;
    if (m_health <= 0) {
        return true;
    }
    if (playerHit) {
        m_isInCombat = true;
    }
    return false;
}

Cube* SentientCube::Move() {
    //path is complete, stop moving restart cycle
    if (m_path.size() == 0) {
        m_isMoving = false;
        return nullptr;
    }
    Cube* deletedCube = nullptr;
    //checks if next cube is destroyed
    Cube* nextCube = m_cubeMap->GetCube(m_path[0].x, m_path[0].y, m_path[0].z);
    if (nextCube != nullptr) {
        if (nextCube->OnHit(false)) {
            m_cubeMap->RemoveCube(nextCube);
            deletedCube = nextCube;
            delete nextCube;
        } else { //if cube is not destroyed, stop repeat movement next cycle
            return nullptr;
        }
    }

    //creates new cube at previous location
    Cube* newCube = new Cube(m_center, m_sideLength);
    newCube->SetTexture(m_textureDiffuse);
    m_cubeMap->RemoveCube(this);
    m_cubeMap->AddCube(newCube);
    m_center = m_path[0];
    this->Clear();
    this->Update();
    m_cubeMap->AddCube(this);
    m_path.erase(m_path.begin());
    return deletedCube;
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
