/** @file SkyBox.hpp
 *  @brief Class to store and handle rendering for a skybox
 *
 *  Loads in and renders a skybox
 *
 *  @author Sam
 *  @bug No known bugs.
 */
#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <vector>
#include "Shader.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.hpp"

class SkyBox{
public:
    SkyBox();
    ~SkyBox();
    // Loads in a skybox based on a list of filepaths for the 6 faces
    void LoadSkyBox(std::vector<std::string> faces);
    // Updates the skybox shader params
    void Update(glm::mat4 projectionMatrix, Camera* camera, glm::vec3 sunColor);
    // Renders the skybox
    void Render();

private:
    // Generates the geometry for the skybox
    void generateSkyBoxGeometry();
    unsigned int m_textureID;
    Shader m_shader;
    unsigned int m_skyboxVAO;
    unsigned int m_skyboxVBO;
};

#endif
