#include "Rendering/Renderer.hpp"
#include <iostream>

// Sets the height and width of our renderer
Renderer::Renderer(unsigned int w, unsigned int h, std::shared_ptr<Player> player){
    m_screenWidth = w;
    m_screenHeight = h;
    m_player = player;

    m_root = nullptr;

    m_skybox.LoadSkyBox({"./common/textures/skybox/right.jpg",
                         "./common/textures/skybox/left.jpg",
                         "./common/textures/skybox/top.jpg",
                         "./common/textures/skybox/bottom.jpg",
                         "./common/textures/skybox/front.jpg",
                         "./common/textures/skybox/back.jpg"});
}

// Sets the height and width of our renderer
Renderer::~Renderer(){
}

//update sun color and skybox color based on position in the sky
void Renderer::Update(){
    m_projectionMatrix = glm::perspective(glm::radians(45.0f),((float)m_screenWidth)/((float)m_screenHeight),0.1f,512.0f);

    Camera* camera = m_player->GetCamera();

    glm::vec3 skyColor = m_orbit.GetSkyColor();
    m_skybox.Update(m_projectionMatrix, camera, skyColor);
    m_orbit.Update(m_projectionMatrix, camera);

    //determines the position of the current lighting object
    float orbitTicks = m_orbit.GetTicks();
    if (orbitTicks > M_PI / 2 && orbitTicks < M_PI * 3 / 2) {
        orbitTicks += M_PI;
    }
    glm::vec3 orbitPos = glm::vec3(250 * sin(orbitTicks),250 * cos(orbitTicks), camera->GetEyeZPosition());

    m_player->UpdateHeldObject(m_projectionMatrix);

    // Perform the update
    if(m_root!=nullptr){
        m_root->Update(m_projectionMatrix, camera, skyColor, orbitPos);
    }
}

void Renderer::Render(){
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor( 0.01f, 0.01f, 0.01f, 1.f );
    // Clear color buffer and Depth Buffer
    // Remember that the 'depth buffer' is our
    // z-buffer that figures out how far away items are every frame
    // and we have to do this every frame!
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    m_orbit.Render();

    m_player->RenderHeldObject();

    // Nice way to debug your scene in wireframe!
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    // Now we render our objects from our scenegraph
    if(m_root!=nullptr){
       m_root->Draw();
    }
    m_skybox.Render();
}
