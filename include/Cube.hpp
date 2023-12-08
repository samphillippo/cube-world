#ifndef CUBE_HPP
#define CUBE_HPP
/** @file Cube.hpp
 *  @brief Cube stores the information associated with a single cube in the environment.
 *
 * 	Contains necessary methods to create, render, and interact with a cube.
 *
 *  @author Sam
 *  @bug No known bugs.
 */

#include "Geometry.hpp"
#include "VertexBufferLayout.hpp"
#include "Object.hpp"

class Cube: public Object {
public:
    // Constructor
    Cube(glm::vec3 center, float sideLength);
    // Destructor
    ~Cube();
    // Initializes position and size of cube
    void Init();
    // get the center of the cube
    glm::vec3 GetCenter();
    // get the side length of the cube
    float GetSideLength();
    //checks a ray for intersection with a cube
    bool IntersectRayWithCube(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, int& hitSide, float &distance) const;
private:
    // Store the objects position
    glm::vec3 m_center;
    // Store the objects side length
    float m_sideLength;
};

#endif
