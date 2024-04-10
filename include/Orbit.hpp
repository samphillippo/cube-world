/** @file Orbit.hpp
 *  @brief Class to update and render the sun and moon.
 *
 *  This class is used to update and render the sun and moon.
 *
 *  @author Sam
 *  @bug No known bugs.
 */
#ifndef ORBIT_HPP
#define ORBIT_HPP

#include "Object.hpp"
#include "Transform.hpp"
#include "Rendering/Shader.hpp"
#include "Camera.hpp"

class Orbit {
public:
    // Constructor
    Orbit();
    // Destructor
    ~Orbit();
    // Update the sun and moon
    void Update(glm::mat4 projectionMatrix, Camera* camera, bool paused);
    // Render the sun or moon
    void Render();
    // get current tick value
    float GetTicks();
    // Gets the color of the sun
    glm::vec3 GetSkyColor();
    // resets orbit to 0 ticks
    void Reset() { m_ticks = 0.0f; }

private:
    // Gets a value representing the suns position in the sky, relative to the peak
    float GetOrbitPosition();
    // object to represent the sun
    Object* m_sunObject;
    //object to represent the moon
    Object* m_moonObject;
    // transform for the sun
    Transform m_sunTransform;
    // transform for the moon
    Transform m_moonTransform;
    // shader for the sun
    Shader m_sunShader;
    // shader for the moon
    Shader m_moonShader;
    // tick counter
    float m_ticks;
    // speed of orbit
    float m_orbitSpeed{0.002f};
};

#endif
