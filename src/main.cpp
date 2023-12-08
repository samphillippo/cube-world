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
	// Run our program forever
	myWorld.Loop();
	// When our program ends, it will exit scope, the
	// destructor will then be called and clean up the program.
	myWorld.SaveWorld(fileName);

	return 0;
}
