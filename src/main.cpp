#include "World.hpp"

int main(int argc, char** argv){
	std::string fileName;
	if (argc < 2) {
        fileName = "";
    } else {
		fileName = argv[1];
	}
	// Create an instance of an object for a SDLGraphicsProgram
	World myWorld;
	myWorld.LoadWorld(fileName);
	// Run our program until exit
	myWorld.Loop();
	// saves world state to file
	myWorld.SaveWorld(fileName);

	return 0;
}
