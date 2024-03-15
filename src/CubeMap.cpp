#include "CubeMap.hpp"

CubeMap::CubeMap() {
}

CubeMap::~CubeMap() {
    for (auto it = coordinateMap.begin(); it != coordinateMap.end(); it++) {
        if (it->second != nullptr) {
            delete it->second;
        }
    }
    coordinateMap.clear();
}

void CubeMap::AddCube(Cube* cube) {
    m_root->AddChild(new SceneNode(cube, "./shaders/cube_vert.glsl", "./shaders/cube_frag.glsl"));
    glm::vec3 center = cube->GetCenter();
    Coordinates coords = {(int)center.x, (int)center.y, (int)center.z};
    coordinateMap[coords] = cube;
}

void CubeMap::RemoveCube(Cube* cube) {
    m_root->RemoveChild(cube);
    glm::vec3 center = cube->GetCenter();
    Coordinates coords = {(int)center.x, (int)center.y, (int)center.z};
    coordinateMap.erase(coords);
}

Cube* CubeMap::GetCube(int x, int y, int z) {
    Coordinates coords = {x, y, z};
    return coordinateMap[coords];
}
