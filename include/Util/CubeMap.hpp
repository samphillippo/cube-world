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
#include <unordered_map>

struct Coordinates {
    int x, y, z;

    bool operator==(const Coordinates& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    bool operator<(const Coordinates& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return z < other.z;
    }
};

struct CoordinatesHash {
    std::size_t operator()(const Coordinates& coord) const {
        size_t h1 = std::hash<int>()(coord.x);
        size_t h2 = std::hash<int>()(coord.y);
        size_t h3 = std::hash<int>()(coord.z);
        return (h1 ^ (h2 << 1)) ^ h3;
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
    const std::unordered_map<Coordinates, Cube*, CoordinatesHash>& getMap() const { return coordinateMap; }
    // Returns a vector of all cubes in the map
    std::vector<Cube*> getCubes() {
        std::vector<Cube*> cubes;
        for (auto it = coordinateMap.begin(); it != coordinateMap.end(); it++) {
            cubes.push_back(it->second);
        }
        return cubes;
    }
private:
    std::unordered_map<Coordinates, Cube*, CoordinatesHash> coordinateMap;
    std::shared_ptr<SceneNode> m_root;
};

#endif
