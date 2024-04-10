/** @file Object.hpp
 *  @brief Information associated with a single OpenGL Object
 *
 *  More...
 *
 *  @author Sam, Mike
 *  @bug No known bugs.
 */
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glad/glad.h>

#include <vector>
#include <string>

#include "Rendering/Shader.hpp"
#include "Rendering/VertexBufferLayout.hpp"
#include "Rendering/Texture.hpp"
#include "Transform.hpp"
#include "Rendering/Geometry.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Purpose:
// An abstraction to create multiple objects
//
//
class Object{
public:
    // Object Constructor
    Object();
    // Object destructor
    ~Object();
    // Load a texture
    void LoadTexture(std::string fileName);
    // Set the texture
    void SetTexture(std::shared_ptr<Texture> texture);
    // Create a textured quad
    void MakeTexturedQuad(std::string fileName);
    // How to draw the object
    virtual void Render();
    // Sets "selected" value
    void SetSelected(bool selected) { m_selected = selected; }
    // Gets "selected" value
    bool GetSelected() { return m_selected; }
    // Sets the color adjustment
    void SetColorAdjustment(glm::vec3 colorAdjustment) { m_colorAdjustment = colorAdjustment; }
    // Gets the color adjustment
    glm::vec3 GetColorAdjustment() { return m_colorAdjustment; }
protected: // Classes that inherit from Object are intended to be overridden.

	// Helper method for when we are ready to draw or update our object
	void Bind();
    // For now we have one buffer per object.
    VertexBufferLayout m_vertexBufferLayout;
    // For now we have one diffuse map and one normal map per object
    std::shared_ptr<Texture> m_textureDiffuse;
    // Store the objects Geometry
	Geometry m_geometry;
    // Whether the object is selected
    bool m_selected;
    // Vector value to be added to final color of rendered object
    glm::vec3 m_colorAdjustment;
};

#endif
