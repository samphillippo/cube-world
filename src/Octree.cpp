#include "Octree.hpp"

Octree::Octree(glm::vec3 center, float size){
    m_center = center;
    m_size = size;
}

Octree::~Octree(){
    // Destructor
}

void Octree::InsertCube(Cube* cube){
    // Inserts a cube into the octree
}

std::vector<Cube*> Octree::GetCubesInArea(const glm::vec3& minBound, const glm::vec3& maxBound){
    std::vector<Cube*> cubes;
    return cubes;
}
