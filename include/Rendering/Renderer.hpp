/** @file Renderer.hpp
 *  @brief Renderer is responsible for drawing.
 *
 * 	Renderer is responsible for drawing everything. It
 *	contains a scenegraph node and a camera. We could
 *	possibly have multiple renderers (if we had multiple
 *	windows for example).
 *
 *	Each renderer thus has it's own camera.
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef RENDERER_HPP
#define RENDERER_HPP

// The glad library helps setup OpenGL extensions.
// This renderer is designed specifically for OpenGL.
#include <glad/glad.h>

#include <vector>

#include "Rendering/SceneNode.hpp"
#include "Camera.hpp"
#include "Rendering/SkyBox.hpp"
#include "Orbit.hpp"
#include "Player.hpp"

class Renderer{
public:
    // The constructor
    // Sets the width and height of the rendererer draws to
    Renderer(unsigned int w, unsigned int h, std::shared_ptr<Player> player);
    // Destructor
    ~Renderer();
    // Update the scene
    void Update(Object* selected);
    // Render the scene
    void Render();
    // Sets the root of our renderer to some node to
    // draw an entire scene graph
    void setRoot(SceneNode* startingNode);
    // resets orbit to 0 ticks
    void resetOrbit() { m_orbit.Reset(); }

protected:
    // Root scene node
    SceneNode* m_root;
    // Store the projection matrix for our camera.
    glm::mat4 m_projectionMatrix;
    // The Player
    std::shared_ptr<Player> m_player;

private:
    // Screen dimension constants
    int m_screenHeight;
    int m_screenWidth;
    // Skybox
    SkyBox m_skybox;
    // Orbitting sun and moon
    Orbit m_orbit;
};

#endif
