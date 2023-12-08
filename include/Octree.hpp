/** @file Octree.hpp
 *  @brief Class implementation for an Octree
 *
 *  Stores an octree of all cubes in the world, and the necessary functions to interact with it
 *
 *  @author Sam
 *  @bug No known bugs.
 */
#ifndef OCTREE_HPP
#define OCTREE_HPP

#include "Cube.hpp"

class Octree{
public:
    Octree(glm::vec3 center, float size);
    ~Octree();
    // Inserts a cube into the octree
    void InsertCube(Cube* cube);
    // Returns a list of all cubes in the octree that are within the given bounds
    std::vector<Cube*> GetCubesInArea(const glm::vec3& minBound, const glm::vec3& maxBound);

private:
    glm::vec3 m_center;
    float m_size;
    std::vector<Cube*> m_cubes;
    std::vector<Octree> m_children;
};


#endif
