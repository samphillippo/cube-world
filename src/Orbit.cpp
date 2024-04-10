#include "Orbit.hpp"
#include "Sphere.hpp"
#include <iostream>

Orbit::Orbit() {
    m_sunObject = new Sphere();
    std::string sunVertexShader = m_sunShader.LoadShader("./shaders/orbit_vert.glsl");
	std::string sunFragmentShader = m_sunShader.LoadShader("./shaders/orbit_frag.glsl");
	m_sunShader.CreateShader(sunVertexShader,sunFragmentShader);
    m_sunObject->LoadTexture("./common/textures/sun.ppm");

    m_moonObject = new Sphere();
    std::string moonVertexShader = m_moonShader.LoadShader("./shaders/orbit_vert.glsl");
	std::string moonFragmentShader = m_moonShader.LoadShader("./shaders/orbit_frag.glsl");
	m_moonShader.CreateShader(moonVertexShader,moonFragmentShader);
    m_moonObject->LoadTexture("./common/textures/rock.ppm");

    m_ticks = 0.0f;
}

Orbit::~Orbit() {
    if (m_sunObject != nullptr) {
        delete m_sunObject;
    }
    if (m_moonObject != nullptr) {
        delete m_moonObject;
    }
}

void Orbit::Update(glm::mat4 projectionMatrix, Camera* camera, bool paused) {
    glm::mat4 view = glm::mat4(glm::mat3(camera->GetWorldToViewmatrix()));
    if (m_ticks < M_PI / 2 || m_ticks > M_PI * 3 / 2) { //day
        m_sunTransform.LoadIdentity();
        m_sunTransform.Translate(250 * sin(m_ticks),250 * cos(m_ticks), 0.0f);

        m_sunShader.Bind();
        // Set the uniforms for the sun
        m_sunShader.SetUniform1i("u_DiffuseMap",0);
        m_sunShader.SetUniformMatrix4fv("model", &m_sunTransform.GetInternalMatrix()[0][0]);
        m_sunShader.SetUniformMatrix4fv("view", &view[0][0]);
        m_sunShader.SetUniformMatrix4fv("projection", &projectionMatrix[0][0]);

        glm::vec3 skyColor = GetSkyColor();
        m_sunShader.SetUniform3f("orbitColor", skyColor.x, skyColor.y, skyColor.z);
        if (!paused) {
            m_ticks += m_orbitSpeed;
        }
    } else { //night
        //updates the position of the moon
        m_moonTransform.LoadIdentity();
        m_moonTransform.Translate(250 * sin(m_ticks + M_PI),250 * cos(m_ticks + M_PI), 0.0f);

        m_moonShader.Bind();
        // Set the uniforms for the moon
        m_moonShader.SetUniform1i("u_DiffuseMap",0);
        m_moonShader.SetUniformMatrix4fv("model", &m_moonTransform.GetInternalMatrix()[0][0]);
        m_moonShader.SetUniformMatrix4fv("view", &view[0][0]);
        m_moonShader.SetUniformMatrix4fv("projection", &projectionMatrix[0][0]);

        m_moonShader.SetUniform3f("orbitColor", 0.0f, 0.0f, 0.0f);
        if (!paused) {
            m_ticks += m_orbitSpeed * 2; //night moves twice as fast
        }
    }
    if (m_ticks > 2.0f * M_PI) {
        m_ticks = 0.0f;
    }
}

void Orbit::Render() {
    if (m_ticks < M_PI / 2 || m_ticks > M_PI * 3 / 2) {
        m_sunShader.Bind();
	    m_sunObject->Render();
    } else {
        m_moonShader.Bind();
	    m_moonObject->Render();
    }
}

float Orbit::GetTicks() {
    return m_ticks;
}

float Orbit::GetOrbitPosition() {
    float pos = abs(m_ticks - M_PI) - M_PI / 2;
    return pos / M_PI * 2;
}

glm::vec3 Orbit::GetSkyColor() {
    float transition = 0.25f;
    glm::vec3 nightBaseColor = glm::vec3(0.4f, 0.4f, 0.4f);
    glm::vec3 dayBaseColor = glm::vec3(1.0f, 0.4f, 0.2f);
    glm::vec3 transitionDiff = dayBaseColor - nightBaseColor;
    float pos = GetOrbitPosition();
    if (m_ticks > M_PI / 2 && m_ticks < M_PI * 3 / 2) {
        //transition period
        if (m_ticks < M_PI / 2 + transition || m_ticks > M_PI * 3 /  - transition) {
            nightBaseColor.r += transitionDiff.r * (1.0f - abs(pos) / transition);
            nightBaseColor.g += transitionDiff.g * (1.0f - abs(pos) / transition);
            nightBaseColor.b += transitionDiff.b * (1.0f - abs(pos) / transition);
        }
        return nightBaseColor;
    }
    //day
    dayBaseColor.g = std::min(1.0f, 0.4f + 1.6f * pos);
    dayBaseColor.b = std::min(1.0f, 0.2f + 2.0f * pos);
    return dayBaseColor;
}
