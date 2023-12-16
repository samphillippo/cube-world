/** @file Player.hpp
 *  @brief Player stores information about the player.
 *
 * 	Player holds information such as player size, position, and speed.
 *  Additionally stores a camera object.
 *
 *  @author Sam
 *  @bug No known bugs.
 */
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Camera.hpp"
#include "Cube.hpp"
#include <memory>

class Player {
public:
    // Constructor
    Player();
    // Destructor
    ~Player();
    Camera* GetCamera() { return m_camera; }
    // Sets player initial state
    void initialize(float x, float y, float z, float height, float sideLength);
    // handles a player mouse movement
    void Look(float x, float y);
    // handles a player movement
    void movePlayer(bool left, bool right, bool forward, bool backward, bool jump, std::vector<Cube*> cubes);
    // swaps between flying and walking mode
    void swapPlayerMode();
    // get whatever object the player is looking at
    Cube* Raycast(const std::vector<Cube*>& cubes, int& hitSide);
    // get the player's selected texture
    std::shared_ptr<Texture> GetSelectedCubeTexture() { return m_selectedCubeTexture; }
    // set the player's selected texture
    void SetSelectedCubeTexture(std::shared_ptr<Texture> texture);
    // update the player's selected object
    void UpdateHeldObject(glm::mat4 projectionMatrix);
    // render the player's selected object
    void RenderHeldObject();

private:
    //gets the direction the player is looking in
    glm::vec3 GetRayDirection();
    // adjusts player position based on collisions
    void handleCollisions(std::vector<Cube*> cubes, bool jump);
    // Position
    glm::vec3 m_position;
    // Size
    float m_height;
    float m_sideLength;
    //Speed
    float m_movementSpeed{0.25f};
    float m_verticalVelocity;
    float m_jumpSpeed{0.15f};
    float m_gravity{0.01f};
    // Mode
    bool m_flyingMode;
    // Camera
    Camera* m_camera;
    // player selected cube texture
    std::shared_ptr<Texture> m_selectedCubeTexture;
    //the object the player holds in their hand
    Object* m_heldObject;
    //shader for the object the player holds in their hand
    Shader m_heldShader;
    // transform for the player's held object
    Transform m_heldTransform;
};

#endif
