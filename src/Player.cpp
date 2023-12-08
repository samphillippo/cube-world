#include "Player.hpp"
#include "Cube.hpp"
#include <iostream>

Player::Player() {
    // Empty
}

Player::~Player() {
    if (m_placeObject != nullptr) {
        delete m_placeObject;
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
    m_placeObject = new Cube(glm::vec3(0.0f), 1.0f);
    std::string vertexShader = m_placeShader.LoadShader("./shaders/cube_vert.glsl");
	std::string fragmentShader = m_placeShader.LoadShader("./shaders/cube_frag.glsl");
	m_placeShader.CreateShader(vertexShader,fragmentShader);
    //creates the transform to put the object in the bottom right corner of the screen
    m_placeTransform.Translate(0.5, -0.3, -1);
    m_placeTransform.Scale(0.2f,0.2f,0.2f);
}

//this whole logic doesn't quite work
void Player::handleIntersections(std::vector<Cube*> intersections, bool jump) {
    float maxX = m_position.x + m_sideLength / 2;
    float minX = m_position.x - m_sideLength / 2;
    float maxZ = m_position.z + m_sideLength / 2;
    float minZ = m_position.z - m_sideLength / 2;
    float maxY = m_position.y;
    float minY = m_position.y - m_height;
    bool standingOnGround = false;
    for (int i = 0; i < intersections.size(); i++) {
        glm::vec3 cubeCenter = intersections[i]->GetCenter();
        float cubeSideLength = intersections[i]->GetSideLength();
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

void Player::movePlayer(bool left, bool right, bool forward, bool backward, bool jump, std::vector<Cube*> intersections) {
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
        handleIntersections(intersections, jump);
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

void Player::SetSelectedCubeTexture(std::shared_ptr<Texture> texture) {
    m_selectedCubeTexture = texture;
    m_placeObject->SetTexture(texture);
}

void Player::UpdatePlaceObject(glm::mat4 projectionMatrix) {
    m_placeShader.Bind();
    m_placeShader.SetUniform1i("selected", 0);
    m_placeShader.SetUniform1i("u_DiffuseMap",0);

    glm::mat4 view;
    m_placeShader.SetUniformMatrix4fv("model", &m_placeTransform.GetInternalMatrix()[0][0]);
    m_placeShader.SetUniformMatrix4fv("view", &view[0][0]);
    m_placeShader.SetUniformMatrix4fv("projection", &projectionMatrix[0][0]);

    //sets uniforms for lighting
    m_placeShader.SetUniform3f("lightColor",1.0f, 1.0f, 1.0f);
    m_placeShader.SetUniform3f("viewPos", 0.0f, 0.0f, 0.0f);
    m_placeShader.SetUniform3f("lightPos", 0.0f, 0.0f, 0.0f);
    m_placeShader.SetUniform1f("ambientIntensity",0.5f);
}

void Player::RenderPlaceObject() {
    m_placeShader.Bind();
	m_placeObject->Render();
}
