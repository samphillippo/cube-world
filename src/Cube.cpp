#include "Cube.hpp"
#include <iostream>

Cube::Cube(glm::vec3 center, float sideLength) {
    // Empty
    m_center = center;
    m_sideLength = sideLength;
    Update();
}

Cube::~Cube() {
    // Empty
}

//NEED CLEAR

void Cube::Clear() {
    m_geometry = Geometry();
    m_vertexBufferLayout = VertexBufferLayout();
}

//TODO: make texture programmatic
//TODO: cleanup hardcodes
void Cube::Update() {
    //0: 1/3
    m_geometry.AddVertex(m_center.x + m_sideLength / 2, m_center.y + m_sideLength / 2, m_center.z + m_sideLength / 2, 0.499217f, 0.749885f); //4+++
    //1: 2/9
    m_geometry.AddVertex(m_center.x + m_sideLength / 2, m_center.y - m_sideLength / 2, m_center.z + m_sideLength / 2, 0.498631f, 0.501320f); //2+-+
    //2: 3/2
    m_geometry.AddVertex(m_center.x - m_sideLength / 2, m_center.y + m_sideLength / 2, m_center.z + m_sideLength / 2, 0.364645f, 0.749570f); //3-++
    //3: 4/5
    m_geometry.AddVertex(m_center.x - m_sideLength / 2, m_center.y - m_sideLength / 2, m_center.z + m_sideLength / 2, 0.364645f, 0.500571f); //1--+
    //4: 5/1
    m_geometry.AddVertex(m_center.x + m_sideLength / 2, m_center.y + m_sideLength / 2, m_center.z - m_sideLength / 2, 0.498370f, 0.997711f); //8++-
    //5: 5/12
    m_geometry.AddVertex(m_center.x + m_sideLength / 2, m_center.y + m_sideLength / 2, m_center.z - m_sideLength / 2, 0.635676, 0.749925f); //8++-
    //6: 6/7
    m_geometry.AddVertex(m_center.x + m_sideLength / 2, m_center.y - m_sideLength / 2, m_center.z - m_sideLength / 2, 0.635676f, 0.500925f); //6+--
    //7: 6/11
    m_geometry.AddVertex(m_center.x + m_sideLength / 2, m_center.y - m_sideLength / 2, m_center.z - m_sideLength / 2, 0.499337, 0.250604f); //6+--
    //8: 7/6
    m_geometry.AddVertex(m_center.x - m_sideLength / 2, m_center.y + m_sideLength / 2, m_center.z - m_sideLength / 2, 0.770971f, 0.749295f); //7-+-
    //9: 7/13
    m_geometry.AddVertex(m_center.x - m_sideLength / 2, m_center.y + m_sideLength / 2, m_center.z - m_sideLength / 2, 0.365873f, 0.998711f); //7-+-
    //10: 7/14
    m_geometry.AddVertex(m_center.x - m_sideLength / 2, m_center.y + m_sideLength / 2, m_center.z - m_sideLength / 2, 0.228977f, 0.748310f); //7-+-
    //11: 8/4
    m_geometry.AddVertex(m_center.x - m_sideLength / 2, m_center.y - m_sideLength / 2, m_center.z - m_sideLength / 2, 0.228806f, 0.503720f); //5---
    //12: 8/8
    m_geometry.AddVertex(m_center.x - m_sideLength / 2, m_center.y - m_sideLength / 2, m_center.z - m_sideLength / 2, 0.771314f, 0.501240f); //5---
    //13: 8/10
    m_geometry.AddVertex(m_center.x - m_sideLength / 2, m_center.y - m_sideLength / 2, m_center.z - m_sideLength / 2, 0.36464f, 0.251714f); //5---

    m_geometry.AddIndex(4);
    m_geometry.AddIndex(2);
    m_geometry.AddIndex(0);
    m_geometry.AddIndex(2);
    m_geometry.AddIndex(11);
    m_geometry.AddIndex(3);
    m_geometry.AddIndex(8);
    m_geometry.AddIndex(6);
    m_geometry.AddIndex(12);
    m_geometry.AddIndex(1);
    m_geometry.AddIndex(13);
    m_geometry.AddIndex(7);
    m_geometry.AddIndex(0);
    m_geometry.AddIndex(3);
    m_geometry.AddIndex(1);
    m_geometry.AddIndex(5);
    m_geometry.AddIndex(1);
    m_geometry.AddIndex(6);
    m_geometry.AddIndex(4);
    m_geometry.AddIndex(9);
    m_geometry.AddIndex(2);
    m_geometry.AddIndex(2);
    m_geometry.AddIndex(10);
    m_geometry.AddIndex(11);
    m_geometry.AddIndex(8);
    m_geometry.AddIndex(5);
    m_geometry.AddIndex(6);
    m_geometry.AddIndex(1);
    m_geometry.AddIndex(3);
    m_geometry.AddIndex(13);
    m_geometry.AddIndex(0);
    m_geometry.AddIndex(2);
    m_geometry.AddIndex(3);
    m_geometry.AddIndex(5);
    m_geometry.AddIndex(0);
    m_geometry.AddIndex(1);

    m_geometry.Gen();

    m_vertexBufferLayout.CreateNormalBufferLayout(m_geometry.GetBufferDataSize(),
                                    m_geometry.GetIndicesSize(),
                                    m_geometry.GetBufferDataPtr(),
                                    m_geometry.GetIndicesDataPtr());
}

glm::vec3 Cube::GetCenter() {
    return m_center;
}

float Cube::GetSideLength() {
    return m_sideLength;
}

bool Cube::OnHit() { //default case, cube should break in one hit
    return true;
}

// Inside your Cube class
bool Cube::IntersectRayWithCube(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, int& hitSide, float &distance) const {
    // Perform the ray-cube intersection test
    glm::vec3 minBound = m_center - glm::vec3(m_sideLength / 2.0f);
    glm::vec3 maxBound = m_center + glm::vec3(m_sideLength / 2.0f);

    float tNear = -FLT_MAX;
    float tFar = FLT_MAX;

    for (int i = 0; i < 3; ++i) {
        if (fabs(rayDirection[i]) < 1e-6) {
            // Ray is parallel to slab, no intersection if outside slab bounds
            if (rayOrigin[i] < minBound[i] || rayOrigin[i] > maxBound[i]) {
                return false;
            }
        } else {
            // Compute intersection t values of the slabs
            float t1 = (minBound[i] - rayOrigin[i]) / rayDirection[i];
            float t2 = (maxBound[i] - rayOrigin[i]) / rayDirection[i];

            // Update tNear and tFar based on the slab intersection
            tNear = glm::max(tNear, glm::min(t1, t2));
            tFar = glm::min(tFar, glm::max(t1, t2));

            // No intersection if tNear > tFar after updating
            if (tNear > tFar) {
                return false;
            }
        }
    }

    // Check if the intersection point is in front of the camera
    if (tFar < 0.0f) {
        return false;
    }

    // Update the intersection point
    glm::vec3 intersectionPoint = rayOrigin + tNear * rayDirection;
    distance = glm::length(intersectionPoint - rayOrigin);

    // Determine which side of the cube is hit
    float epsilon = 1e-6;
    for (int i = 0; i < 3; ++i) {
        if (fabs(intersectionPoint[i] - minBound[i]) < epsilon) {
            hitSide = i * 2; // Even index indicates a face along the negative axis
            return true;
        } else if (fabs(intersectionPoint[i] - maxBound[i]) < epsilon) {
            hitSide = i * 2 + 1; // Odd index indicates a face along the positive axis
            return true;
        }
    }

    return false;
}
