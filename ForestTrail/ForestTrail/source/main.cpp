#include "GameEngine.h"

#include <sstream>

using namespace GE;

int main(int argc, char* argv[]) {
	// Create a game engine object
	GameEngine ge;

	// Initialise it
	if (!ge.init()) {
		display_info_message("Couldn't start SDL. Check console output for more details");

		return -1;
	}

	// Variables used to store the time at two points in the program
	Uint32 last_time = SDL_GetTicks(), current_time = 0;

	// Simple frame counter
	int frame_count = 0;

	// Main loop
	while (ge.keep_running()) {
		// Update game state
		ge.update();

		// Handle input
		ge.processInput();

		// Draw
		ge.draw();

		// Increment the frame counter each update
		frame_count++;

		// Get the time at this point
		current_time = SDL_GetTicks();
		
		// Is the difference 1000?
		if (current_time - last_time > 1000) {
			// Create a variable to store the string
			std::ostringstream msg;

			// Create message based on the value in frame_count
			msg << "FPS: " << frame_count;

			// Display the message in the window title
			ge.setWindowTitle(msg.str().c_str());

			// Reset the frame counter
			frame_count = 0;

			// Update time variable for next count
			last_time = current_time;
		}
	}

	// Destroy game engine
	ge.shutdown();

	return 0;
}