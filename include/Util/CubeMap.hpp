/** @file CubeMap.hpp
 *  @brief CubeMap is a class used to store and provide access to a 3D grid of cubes.
 *
 * 	Contains necessary methods to add to and remove from the grid, as well as to query the grid.
 *
 *  @author Sam
 *  @bug No known bugs.
 */
#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

#include "Cube.hpp"
#include "Rendering/SceneNode.hpp"
#include <map>

struct Coordinates {
    int x, y, z;

    // Comparison operator for sorting
    bool operator<(const Coordinates& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return z < other.z;
    }
    bool operator==(const Coordinates& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

class CubeMap {
public:
    // Constructor
    CubeMap();
    // Destructor
    ~CubeMap();
    // Adds a cube to the map
    void AddCube(Cube* cube);
    // Removes a cube from the map
    void RemoveCube(Cube* cube);
    // Returns the cube at the given coordinates
    Cube* GetCube(int x, int y, int z);
    // Sets the root value for this CubeMap
    void setRoot(std::shared_ptr<SceneNode> root) { m_root = root; }
    // Returns the map of cubes
    const std::map<Coordinates, Cube*>& getMap() const { return coordinateMap; }
private:
    std::map<Coordinates, Cube*> coordinateMap;
    std::shared_ptr<SceneNode> m_root;
};

#endif
