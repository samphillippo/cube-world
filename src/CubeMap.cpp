#include "CubeMap.hpp"

CubeMap::CubeMap() {
}

CubeMap::~CubeMap() {
    for (auto it = coordinateMap.begin(); it != coordinateMap.end(); it++) {
        delete it->second;
    }
    coordinateMap.clear();
}

void CubeMap::AddCube(Cube* cube) {
    glm::vec3 center = cube->GetCenter();
    Coordinates coords = {(int)center.x, (int)center.y, (int)center.z};
    coordinateMap[coords] = cube;
}

void CubeMap::RemoveCube(Cube* cube) {
    glm::vec3 center = cube->GetCenter();
    Coordinates coords = {(int)center.x, (int)center.y, (int)center.z};
    coordinateMap.erase(coords);
}

Cube* CubeMap::GetCube(int x, int y, int z) {
    Coordinates coords = {x, y, z};
    return coordinateMap[coords];
}
