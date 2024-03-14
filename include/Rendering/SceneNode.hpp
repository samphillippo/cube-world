/** @file SceneNode.hpp
 *  @brief SceneNode helps organize a large 3D graphics scene.
 *
 *  SceneNode helps organize a large 3D graphics scene.
 *  The traversal of the tree takes place starting from
 *  a single SceneNode (typically called root).
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <vector>

#include "Object.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Rendering/Shader.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

class SceneNode{
public:
    // A SceneNode is created by taking
    // a pointer to an object.
    SceneNode(Object* ob, std::string vertexShader, std::string fragmentShader);
    // Our destructor takes care of destroying
    // all of the children within the node.
    // Now we do not have to manage deleting
    // each individual object.
    ~SceneNode();
    // Adds a child node to our current node.
    void AddChild(SceneNode* n);
    // Removes the child with the given object
    void RemoveChild(Object* o);
    // Draws the current SceneNode
    void Draw();
    // Updates the current SceneNode
    void Update(glm::mat4 projectionMatrix, Camera* camera, glm::vec3 skyColor, glm::vec3 orbitPos, Object* selected);
    // Returns the local transformation transform
    // Remember that local is local to an object, where it's center is the origin.
    Transform& GetLocalTransform();
    // Returns a SceneNode's world transform
    Transform& GetWorldTransform();
    // For now we have one shader per Node.
    Shader m_shader;

protected:
    // Parent
    SceneNode* m_parent;
private:
    // Children holds all a pointer to all of the descendents
    // of a particular SceneNode. A pointer is used because
    // we do not want to hold or make actual copies.
    std::vector<SceneNode*> m_children;
    // The object stored in the scene graph
    Object* m_object;
    // Each SceneNode nodes locals transform.
    Transform m_localTransform;
    // We additionally can store the world transform
    Transform m_worldTransform;
};

#endif
