#include "World.hpp"
#include "Camera.hpp"
#include "AI/PathPlacer.hpp"
#include "AI/BlockBreaker.hpp"
#include "AI/BrickBuilder.hpp"
#include "AI/GroundGrower.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

World::World(){
	// Initialization flag
	bool success = true;
	// String to hold any errors that occur.
	std::stringstream errorStream;
	// The window we'll be rendering to
	m_window = NULL;

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	}
	else{
		//Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		// We want to request a double buffer for smooth updating.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		//Create window
		m_window = SDL_CreateWindow( "World", //TODO: pull name from given file
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                m_windowWidth,
                                m_windowHeight,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

		// Check if Window did not create.
		if( m_window == NULL ){
			errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		//Create an OpenGL Graphics Context
		m_openGLContext = SDL_GL_CreateContext( m_window );
		if( m_openGLContext == NULL){
			errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		// Initialize GLAD Library
		if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
			errorStream << "Failed to iniitalize GLAD\n";
			success = false;
		}

		//Initialize OpenGL
		if(!InitGL()){
			errorStream << "Unable to initialize OpenGL!\n";
			success = false;
		}
  	}

    // If initialization did not work, then print out a list of errors in the constructor.
    if(!success){
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors=errorStream.str();
        SDL_Log("%s\n",errors.c_str());
    }else{
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

	// SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
	GetOpenGLVersionInfo();
}

// Proper shutdown of SDL and destroy initialized objects
World::~World(){
    if(m_renderer!=nullptr){
        delete m_renderer;
    }

    //Destroy window
	SDL_DestroyWindow( m_window );
	// Point m_window to NULL to ensure it points to nothing.
	m_window = nullptr;
	//Quit SDL subsystems
	SDL_Quit();
}

void World::LoadWorld(std::string filename) {
    if (filename == "") {
        //creates all our textures
        std::shared_ptr<Texture> defaultTexture = std::make_shared<Texture>();
        defaultTexture->LoadTexture("./common/textured_cube/cube.ppm");
        m_textures.push_back(defaultTexture);

        std::shared_ptr<Texture> dirtTexture = std::make_shared<Texture>();
        dirtTexture->LoadTexture("./common/textures/detailmap.ppm");
        m_textures.push_back(dirtTexture);

        std::shared_ptr<Texture> grassTexture = std::make_shared<Texture>();
        grassTexture->LoadTexture("./common/textures/grass.ppm");
        m_textures.push_back(grassTexture);

        std::shared_ptr<Texture> brickTexture = std::make_shared<Texture>();
        brickTexture->LoadTexture("./common/textures/brick.ppm");
        m_textures.push_back(brickTexture);

        std::shared_ptr<Texture> rockTexture = std::make_shared<Texture>();
        rockTexture->LoadTexture("./common/textures/rock.ppm");
        m_textures.push_back(rockTexture);

        std::shared_ptr<Texture> breakerTexture = std::make_shared<Texture>();
        breakerTexture->LoadTexture("./common/textures/terrain.ppm");
        m_textures.push_back(breakerTexture);

        //creates our cube map
        m_cubeMap = std::make_shared<CubeMap>();

        //creates our player
        m_player = std::make_shared<Player>();
        m_player->initialize(0.0f,3.0f,0.0f,1.8f,0.8f, m_cubeMap);
        m_player->SetHeldObjectTexture(m_textures[1]);

        m_root = std::make_shared<SceneNode>(nullptr, "", "");
        m_cubeMap->setRoot(m_root);
        m_noiseMap = std::make_shared<PerlinNoise>();

        //--------------------ADDS SENTIENT CUBES--------------------
        glm::vec3 pathPlacerPos = glm::vec3(4,0,4);
        pathPlacerPos.y = m_noiseMap->GetNoiseValue(pathPlacerPos.x, pathPlacerPos.z) + 1;
        SentientCube* pathPlacer = new PathPlacer(pathPlacerPos, 1.0f, m_cubeMap);
        pathPlacer->SetTexture(rockTexture);
        m_cubeMap->AddCube(pathPlacer);

        glm::vec3 pathPlacer2Pos = glm::vec3(4,0,0);
        pathPlacer2Pos.y = m_noiseMap->GetNoiseValue(pathPlacer2Pos.x, pathPlacer2Pos.z) + 1;
        SentientCube* pathPlacer2 = new PathPlacer(pathPlacer2Pos, 1.0f, m_cubeMap);
        pathPlacer2->SetTexture(rockTexture);
        m_cubeMap->AddCube(pathPlacer2);

        for (int i = 0; i < 10; i++) {
            glm::vec3 blockBreakerPos = glm::vec3(-3,0,i);
            blockBreakerPos.y = m_noiseMap->GetNoiseValue(blockBreakerPos.x, blockBreakerPos.z) + 1;
            SentientCube* blockBreaker = new BlockBreaker(blockBreakerPos, 1.0f, m_cubeMap);
            blockBreaker->SetTexture(breakerTexture);
            m_cubeMap->AddCube(blockBreaker);
        }

        //GroundGrower initialization
        int initialGroundSize = 12;
        std::vector<glm::vec3> groundGrowerDirs = { glm::vec3(1,0,0), glm::vec3(-1,0,0), glm::vec3(0,0,1), glm::vec3(0,0,-1) };
        for (int i = 0; i < groundGrowerDirs.size(); i++) {
            //determines the starting position of the ground grower
            glm::vec3 expandDir = glm::vec3(-groundGrowerDirs[i].z, 0, groundGrowerDirs[i].x);
            glm::vec3 startPos = glm::vec3(-groundGrowerDirs[i].x + expandDir.x, 0, -groundGrowerDirs[i].z + expandDir.z);
            startPos *= initialGroundSize / 2;
            startPos += expandDir;
            startPos.y = m_noiseMap->GetNoiseValue(startPos.x, startPos.z);

            SentientCube* groundGrower = new GroundGrower(startPos, 1.0f, m_cubeMap, m_noiseMap, groundGrowerDirs[i], initialGroundSize + 1);
            groundGrower->SetTexture(grassTexture);
            m_cubeMap->AddCube(groundGrower);
        }

        glm::vec3 brickBuilderPos = glm::vec3(-4,0,4);
        brickBuilderPos.y = m_noiseMap->GetNoiseValue(brickBuilderPos.x, brickBuilderPos.z) + 1;
        SentientCube* brickBuilder = new BrickBuilder(brickBuilderPos, 1.0f, m_cubeMap, m_noiseMap);
        brickBuilder->SetTexture(brickTexture);
        m_cubeMap->AddCube(brickBuilder);
        //-------------------------------------------------------------

        //creates initial area for player to stand on
        std::vector<glm::vec3> initialGroundCubes = m_noiseMap->GetInitialGroundCubes(12);
        for (int i = 0; i < initialGroundCubes.size(); i++) {
            Cube* groundCube = new Cube(initialGroundCubes[i], 1.0f);
            groundCube->SetTexture(grassTexture);
            m_cubeMap->AddCube(groundCube);
        }

    } else {
        parseWorldFile(filename);
    }
}

//TODO: parse the world file
void World::parseWorldFile(std::string filename) {
    std::ifstream file(filename);
    std::string line;
    while(std::getline(file, line)) {
        std::cout << line << std::endl;
    }
}

void World::SaveWorld(std::string filename) {
    std::ofstream file(filename);
    file << "Hello World!" << std::endl;
    //3d vector?
    //track min/max xyz
    //map vec3 to cube
    //when we've parsed all, traverse by y then x then z
}

// Initialize OpenGL
// Setup any of our shaders here.
bool World::InitGL(){
	//Success flag
	bool success = true;

	return success;
}

//Loops forever!
void World::Loop(){
    if (m_player != nullptr) {
        m_renderer = new Renderer(m_windowWidth, m_windowHeight, m_player);
    }
    //Maybe make the spawn point the root node?
    m_renderer->setRoot(m_root);

    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // if this is true, sentient blocks + orbits do not tick
    bool paused = false;
    // Enable text input
    SDL_StartTextInput();
    // Center our mouse
    SDL_WarpMouseInWindow(m_window,m_windowWidth/2,m_windowHeight/2);
    // Enable relative mouse mode
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Set the camera speed for how fast we move.
    float cameraSpeed = 5.0f;

    // While application is running
    while(!quit){
        if (!paused) {
            //tick all cubes
            std::vector<Cube*> allCubes = m_cubeMap->getCubes();
            for (int i = 0; i < allCubes.size(); i++) {
                Cube* cube = allCubes[i];
                if (cube == nullptr) {
                    continue;
                }
                Cube* deletedCube = cube->OnTick();
                if (deletedCube != nullptr) {
                    for (int j = 0; j < allCubes.size(); j++) {
                        if (allCubes[j] == deletedCube) {
                            allCubes.erase(allCubes.begin() + j);
                            if (j < i) {
                                i--;
                            }
                            break;
                        }
                    }
                }
            }
        }

        //detects if a cube is being looked at
        int hitSide;
        Cube* selected = m_player->Raycast(hitSide);

        // handle player input
        quit = handleInput(selected, hitSide, paused);

        // Update our scene through our renderer
        m_renderer->Update(paused);
        // Render our scene using our selected renderer
        m_renderer->Render();
        // Delay to slow things down just a bit!
        SDL_Delay(25);  // TODO: You can change this or implement a frame
                        // independent movement method if you like.
      	//Update screen of our specified window
      	SDL_GL_SwapWindow(GetSDLWindow());
	}
    //Disable text input
    SDL_StopTextInput();
}

// Get Pointer to Window
SDL_Window* World::GetSDLWindow(){
  return m_window;
}

// Helper Function to get OpenGL Version Information
void World::GetOpenGLVersionInfo(){
	SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}

bool World::handleInput(Cube* selected, int hitSide, bool& paused) {
    SDL_Event e;
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    static int mouseX=m_windowWidth/2;
    static int mouseY=m_windowHeight/2;
    //Handle events on queue
    while(SDL_PollEvent( &e ) != 0) {
        // User posts an event to quit
        // An example is hitting the "x" in the corner of the window.
        if(e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)){
            return true;
        } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f) {
            m_player->swapPlayerMode();
        } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_i) {
            handleRightClick(selected, hitSide);
        } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r) {
            m_renderer->resetOrbit();
        } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p) {
            paused = !paused;
        }
        // Handle keyboard input for the camera class
        if(e.type==SDL_MOUSEMOTION){
            // Handle mouse movements
            mouseX+=e.motion.xrel;
            mouseY+=e.motion.yrel;
            m_player->Look(mouseX, mouseY);
        }
        //if the player clicks the left mouse button, delete the cube they are looking at
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (selected != nullptr) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    handleLeftClick(selected);
                } else if (e.button.button == SDL_BUTTON_RIGHT) {
                    handleRightClick(selected, hitSide);
                }
            }
        }
    }
    if (keyboardState[SDL_SCANCODE_0]) {
        m_player->SetHeldObjectTexture(m_textures[0]);
    } else if (keyboardState[SDL_SCANCODE_1]) {
        m_player->SetHeldObjectTexture(m_textures[1]);
    } else if (keyboardState[SDL_SCANCODE_2]) {
        m_player->SetHeldObjectTexture(m_textures[2]);
    } else if (keyboardState[SDL_SCANCODE_3]) {
        m_player->SetHeldObjectTexture(m_textures[3]);
    } else if (keyboardState[SDL_SCANCODE_4]) {
        m_player->SetHeldObjectTexture(m_textures[4]);
    }

    m_player->movePlayer(keyboardState[SDL_SCANCODE_A], keyboardState[SDL_SCANCODE_D],
                            keyboardState[SDL_SCANCODE_W], keyboardState[SDL_SCANCODE_S],
                            keyboardState[SDL_SCANCODE_SPACE]);

    return false;
}

void World::handleLeftClick(Cube* selected) {
    //if the cube is not a sentient cube, delete it
    if (selected != nullptr) {
        if (selected->OnHit()) {
            m_cubeMap->RemoveCube(selected);
            delete selected;
        }
    }
}

void World::handleRightClick(Cube* selected, int hitSide) {
    glm::vec3 pos = selected->GetCenter();
    float sideLength = selected->GetSideLength();
    switch (hitSide) {
        case 0:
            pos.x -= sideLength;
            break;
        case 1:
            pos.x += sideLength;
            break;
        case 2:
            pos.y -= sideLength;
            break;
        case 3:
            pos.y += sideLength;
            break;
        case 4:
            pos.z -= sideLength;
            break;
        case 5:
            pos.z += sideLength;
            break;
    }
    Cube* newCube = new Cube(pos, sideLength);
    std::shared_ptr<Texture> heldTexture = m_player->GetHeldObjectTexture();
    newCube->SetTexture(heldTexture);
    m_cubeMap->AddCube(newCube);
}
