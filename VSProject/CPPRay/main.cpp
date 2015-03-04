#include <iostream>
#include <string>
#include "sdl2/SDL.h"
#include "display.h"
#include "engine.h"
#include "vec3.h"
#include "quaternion.h"
#include "mat4.h"

int main(int argc, char** argv)
{
	// Boolean which is used to check if the program is running or not
	bool running = true;

	// Initialize SDL2, close the program if that fails
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	else
	{
		std::cout << "SDL_Init has been succesfully executed." << std::endl;
	}

	// Initialize the main display object by loading it into the stack
	Display display("C++ Raytracer", 512, 512);

	// Initialize the main engine object that handles the tracing
	Engine engine(&display);

	// Create SDL_Event object
	SDL_Event event;

	// Initialize deltaTime related variables
	int frames = 0;
	int currentFrame = 0;
	int lastFrame = SDL_GetTicks();
	float deltaTime = 0;
	float frameTime = 0;

	while (running)
	{
		// Calculate delta-time
		currentFrame = SDL_GetTicks();
		deltaTime = 0.9f * deltaTime + 0.1f * (float) (currentFrame - lastFrame) / 1000;
		lastFrame = currentFrame;

		// Calculate frames per second
		frameTime = 1.0f / deltaTime;

		// Display info after 1000 frames
		if (frames >= 1000)
		{
			std::cout << "deltaTime: " << deltaTime << "s" << " | frameTime: " << frameTime << "FPS" << std::endl;
			frames = 0;
		}

		// Render everything
		display.clear(0x00000000);
		engine.update(deltaTime);
		engine.render();
		display.render();

		// Process SDL events
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		}

		// Increment frame counter
		frames++;
	}

	// Quit SDL2 and return 0 to indicate the program executed succesfully
	SDL_Quit();
	return 0;
}