#include "GameEngine.h"

#include <sstream>

using namespace GE;

int main(int argc, char* argv[]) {
	// Create a game engine object
	GameEngine ge;

	// SDL_GetTicks() provides millisecond precision timing (sufficient for profiling)
	Uint32 start = SDL_GetTicks();
	// Initialise it
	if (!ge.init()) {
		display_info_message("Couldn't start SDL. Check console output for more details");

		return -1;
	}
	Uint32 end = SDL_GetTicks();
	printf("ge.init() took %u ms\n", end - start);


	// Variables used for FPS timing
	Uint32 last_time = SDL_GetTicks(), current_time = 0;

	Uint32 update_total = 0, draw_total = 0;

	// Simple frame counters (for fps and code profiling)
	int fps_frame_count = 0;
	int profiling_frame_count = 0;

	// Main loop
	while (ge.keep_running()) {
		// Record the time before and after update()
		Uint32 start_update = SDL_GetTicks();
		// Update game state
		ge.update();
		Uint32 end_update = SDL_GetTicks();
		// Accumulate the time taken for one update call
		update_total += (end_update - start_update);

		// Record the time before and after draw()
		Uint32 start_draw = SDL_GetTicks();
		// Draw
		ge.draw();
		Uint32 end_draw = SDL_GetTicks();
		// Accumulate the time taken for one draw call
		draw_total += (end_draw - start_draw);

		// Increment the frame counters each update
		fps_frame_count++;
		profiling_frame_count++;

		// Get the time at this point
		current_time = SDL_GetTicks();

		// Update window title once per second with current FPS
		if (current_time - last_time > 1000) {
			// Create a variable to store the string
			std::ostringstream msg;

			// Create message based on the value in frame_count
			msg << "Forest Trail | FPS: " << fps_frame_count;

			// Display the message in the window title
			ge.setWindowTitle(msg.str().c_str());

			// Reset the frame counter
			fps_frame_count = 0;

			// Update time variable for next count
			last_time = current_time;
		}

		// Every 100 frames, get the average time taken to complete the update and draw functions
		// Use 100 frames to produce stable average timings
		if (profiling_frame_count == 100) {
			printf("Average ge.update(): %.2f ms\n", update_total / (float)profiling_frame_count);
			printf("Average ge.draw(): %.2f ms\n", draw_total / (float)profiling_frame_count);

			// Reset all the related variables
			update_total = 0;
			draw_total = 0;
			profiling_frame_count = 0;
		}
	}

	// Destroy game engine
	ge.shutdown();

	return 0;
}