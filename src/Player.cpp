#include "Player.hpp"
#include "Cube.hpp"
#include <iostream>

Player::Player() {
    // Empty
}

Player::~Player() {
    if (m_heldObject != nullptr) {
        delete m_heldObject;
    }
    if (m_camera != nullptr) {
        delete m_camera;
    }
}

void Player::initialize(float x, float y, float z, float height, float sideLength) {
    // Set the position
    m_position = glm::vec3(x, y, z);
    // Set the size
    m_height = height;
    m_sideLength = sideLength;

    m_verticalVelocity = 0.0f;
    m_flyingMode = false;

    // Create the camera
    m_camera = new Camera(x, y, z);

    //creates the model for the object the player holds
    m_heldObject = new Cube(glm::vec3(0.0f), 1.0f);
    std::string vertexShader = m_heldShader.LoadShader("./shaders/cube_vert.glsl");
	std::string fragmentShader = m_heldShader.LoadShader("./shaders/cube_frag.glsl");
	m_heldShader.CreateShader(vertexShader,fragmentShader);
    //creates the transform to put the object in the bottom right corner of the screen
    m_heldTransform.Translate(0.5, -0.3, -1);
    m_heldTransform.Scale(0.2f,0.2f,0.2f);
}

void Player::movePlayer(bool left, bool right, bool forward, bool backward, bool jump, std::vector<Cube*> cubes) {
    glm::vec3 direction = GetRayDirection();
    if (!m_flyingMode) {
        direction.y = 0.0f;
    }
    if (left && !right) {
        glm::vec3 rightVector = glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f));
        m_position -= rightVector * m_movementSpeed;
    }
    if (right && !left) {
        glm::vec3 rightVector = glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f));
        m_position += rightVector * m_movementSpeed;
    }
    if (forward && !backward) {
        m_position += direction * m_movementSpeed;
    }
    if (backward && !forward) {
        m_position -= direction * m_movementSpeed;
    }
    if (!m_flyingMode) {
        handleCollisions(cubes, jump);
    }
    m_camera->SetCameraEyePosition(m_position.x, m_position.y, m_position.z);
}

void Player::Look(float x, float y) {
    m_camera->MouseLook(x, y);
}

void Player::swapPlayerMode() {
    m_flyingMode = !m_flyingMode;
    m_verticalVelocity = 0.0f;
}

void Player::handleCollisions(std::vector<Cube*> cubes, bool jump) {
    float maxX = m_position.x + m_sideLength / 2;
    float minX = m_position.x - m_sideLength / 2;
    float maxZ = m_position.z + m_sideLength / 2;
    float minZ = m_position.z - m_sideLength / 2;
    float maxY = m_position.y;
    float minY = m_position.y - m_height;
    bool standingOnGround = false;
    for (int i = 0; i < cubes.size(); i++) {
        glm::vec3 cubeCenter = cubes[i]->GetCenter();
        float cubeSideLength = cubes[i]->GetSideLength();
        float cubeMaxX = cubeCenter.x + cubeSideLength / 2;
        float cubeMinX = cubeCenter.x - cubeSideLength / 2;
        float cubeMaxZ = cubeCenter.z + cubeSideLength / 2;
        float cubeMinZ = cubeCenter.z - cubeSideLength / 2;
        float cubeMaxY = cubeCenter.y + cubeSideLength / 2;
        float cubeMinY = cubeCenter.y - cubeSideLength / 2;
        float overlapX = std::min(maxX - cubeMinX, cubeMaxX - minX);
        float overlapZ = std::min(maxZ - cubeMinZ, cubeMaxZ - minZ);
        float overlapY = std::min(maxY - cubeMinY, cubeMaxY - minY);
        //checks if the player is close enough to the ground to jump
        if (cubeMaxY > minY - 0.1 && cubeMinY < maxY && overlapX > 0 && overlapZ > 0) {
            standingOnGround = true; //TODO: could allow player to jump while sliding on a wall...
        }
        //the issue is it always sees y direction as smallest overlap
        if (overlapX < overlapZ && overlapX < overlapY) {
            // Resolve X-axis collision
            if (maxX > cubeMinX && minX < cubeMinX) {
                m_position.x = cubeMinX - m_sideLength / 2;
            } else if (minX < cubeMaxX && maxX > cubeMaxX) {
                m_position.x = cubeMaxX + m_sideLength / 2;
            }
        } else if (overlapZ < overlapX && overlapZ < overlapY) {
            // Resolve Z-axis collision
            if (maxZ > cubeMinZ && minZ < cubeMinZ) {
                m_position.z = cubeMinZ - m_sideLength / 2;
            } else if (minZ < cubeMaxZ && maxZ > cubeMaxZ) {
                m_position.z = cubeMaxZ + m_sideLength / 2;
            }
        } else {
            // Resolve Y-axis collision
            if (maxY > cubeMinY && minY < cubeMinY) {
                m_position.y = cubeMinY;
            } else if (minY < cubeMaxY && maxY > cubeMaxY) {
                m_position.y = cubeMaxY + m_height;
            }
        }
    }

    //gravity/jump logic
    if (standingOnGround) {
        if (jump) {
            m_verticalVelocity = m_jumpSpeed;
            m_position.y += m_verticalVelocity;
        } else {
            m_verticalVelocity = 0.0f;
        }
    } else {
        m_verticalVelocity -= m_gravity;
        m_verticalVelocity = std::max(m_verticalVelocity, -1.5f * m_jumpSpeed);
        m_position.y += m_verticalVelocity;
    }
}

////////////////////////////////////////////////
// RAYCASTING
////////////////////////////////////////////////
Cube* Player::Raycast(const std::vector<Cube*>& cubes, int& hitSide) {
    // Get the ray direction from the center of the screen
    glm::vec3 rayDirection = GetRayDirection();

    float minIntersectionDistance = FLT_MAX;
    Cube* selected = nullptr;
    int hitSideTemp;
    // Iterate over cubes and check for intersections
    for (int i = 0; i < cubes.size(); i++) {
        // Perform intersection test
        float intersectionDistance;
        Cube* cube = cubes[i];
        if (cube->IntersectRayWithCube(m_position, rayDirection, hitSideTemp, intersectionDistance)) {
            if (intersectionDistance < minIntersectionDistance) {
                minIntersectionDistance = intersectionDistance;
                selected = cube;
                hitSide = hitSideTemp;
            }
        }
    }

    return selected;
}

glm::vec3 Player::GetRayDirection() {
    return glm::vec3(m_camera->GetViewXDirection(), m_camera->GetViewYDirection(), m_camera->GetViewZDirection());
}


////////////////////////////////////////////////
// HELD OBJECT
////////////////////////////////////////////////
void Player::SetSelectedCubeTexture(std::shared_ptr<Texture> texture) {
    m_selectedCubeTexture = texture;
    m_heldObject->SetTexture(texture);
}

void Player::UpdateHeldObject(glm::mat4 projectionMatrix) {
    m_heldShader.Bind();
    m_heldShader.SetUniform1i("selected", 0);
    m_heldShader.SetUniform1i("u_DiffuseMap",0);

    //Sets MVP matrices
    glm::mat4 view;
    m_heldShader.SetUniformMatrix4fv("model", &m_heldTransform.GetInternalMatrix()[0][0]);
    m_heldShader.SetUniformMatrix4fv("view", &view[0][0]);
    m_heldShader.SetUniformMatrix4fv("projection", &projectionMatrix[0][0]);

    //sets uniforms for lighting
    m_heldShader.SetUniform3f("lightColor",1.0f, 1.0f, 1.0f);
    m_heldShader.SetUniform3f("viewPos", 0.0f, 0.0f, 0.0f);
    m_heldShader.SetUniform3f("lightPos", 0.0f, 0.0f, 0.0f);
    m_heldShader.SetUniform1f("ambientIntensity",0.5f);
}

void Player::RenderHeldObject() {
    m_heldShader.Bind();
	m_heldObject->Render();
}
