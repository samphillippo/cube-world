/** @file World.hpp
 *  @brief Class to store world state and render.
 *
 *  This class contains a representation of a cubeworld, and the necessary functions to render and interact with it
 *
 *  @author Sam, Mike
 *  @bug No known bugs.
 */
#ifndef WORLD_HPP
#define WORLD_HPP

// ==================== Libraries ==================
// Depending on the operating system we use
// The paths to SDL are actually different.
// The #define statement should be passed in
// when compiling using the -D argument.
// This gives an example of how a programmer
// may support multiple platforms with different
// dependencies.
#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif

// Include the 'Renderer.hpp' which deteremines what
// the graphics API is going to be for OpenGL
#include "Rendering/Renderer.hpp"
#include "Player.hpp"
#include "Cube.hpp"
#include "AI/SentientCube.hpp"

// Purpose:
// This class sets up a full graphics program using SDL
//
//
//
class World {
public:

    // Constructor
    World();
    // Destructor
    ~World();
    // Setup OpenGL
    bool InitGL();
    // Load in an initial world state from a file
    void LoadWorld(std::string filename);
    // Loop that runs forever
    void Loop();
    // Get Pointer to Window
    SDL_Window* GetSDLWindow();
    // Helper Function to Query OpenGL information.
    void GetOpenGLVersionInfo();
    // Save the current world state to a file
    void SaveWorld(std::string filename);

private:
    // Helper function to handle input events
    bool handleInput(Cube* selected, int hitSide);
    // Helper function to handle mouse left click
    void handleLeftClick(Cube* selected);
    // Helper function to parse a world file
    void parseWorldFile(std::string filename);
    // add a new cube to the world based on the given information
    void addCube(Cube* cube, int face);
    // delete the given cube from the world
    void deleteCube(Cube* cube);
	// The Renderer responsible for drawing objects
	// in OpenGL (Or whatever Renderer you choose!)
	Renderer* m_renderer;
    // The window we'll be rendering to
    SDL_Window* m_window ;
    // Window width and height
    unsigned int m_windowWidth{1280};
    unsigned int m_windowHeight{720};
    // OpenGL context
    SDL_GLContext m_openGLContext;
    // Our player
    std::shared_ptr<Player> m_player;
    // Our textures
    std::vector<std::shared_ptr<Texture>> m_textures;
    // All cubes in the scene
    std::vector<Cube*> m_cubes;
    // All sentient cubes in the scene
    std::vector<SentientCube*> m_sentientCubes; //how can we store cubes and sentient cubes in the same list?
    // Our root node
    SceneNode* m_root;
};

#endif
