#include <iostream>
#include <string>
#include "sdl2/SDL.h"
#include "config.h"
#include "display.h"
#include "engine.h"

using namespace math;

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
	Display display("C++ Raytracer", WIDTH, HEIGHT, SCALE);

	// Initialize the main camera
	Camera camera = Camera(vec3(0, 0, 2), quaternion().identity().createFromAxisAngle(0, 1, 0, 45), vec3(1), 64, 256);

	// Initialize the main engine object that handles the tracing
	Engine engine(&display, &camera);

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
		if (frames >= 100)
		{
			std::cout << "deltaTime: " << deltaTime << "s" << " | frameTime: " << frameTime << "FPS" << std::endl;
			frames = 0;
		}

		// Render everything
		engine.update(deltaTime);
		display.clear(0x00000000);
		engine.render();
		display.render();

		// Process SDL events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = false;
					break;

				// Camera movement
				case SDLK_w:
					camera.move(camera.getTransform().getRotation().getForwardVector(), deltaTime);
					break;
				case SDLK_s:
					camera.move(camera.getTransform().getRotation().getForwardVector().negate(), deltaTime);
					break;
				case SDLK_a:
					camera.move(camera.getTransform().getRotation().getRightVector(), deltaTime);
					break;
				case SDLK_d:
					camera.move(camera.getTransform().getRotation().getRightVector().negate(), deltaTime);
					break;
				case SDLK_r:
					camera.move(camera.getTransform().getRotation().getUpVector().negate(), deltaTime);
					break;
				case SDLK_f:
					camera.move(camera.getTransform().getRotation().getUpVector(), deltaTime);
					break;

				// Camera orientation
				case SDLK_UP:
					camera.rotate(camera.getTransform().getRotation().getRightVector(), deltaTime);
					break;
				case SDLK_DOWN:
					camera.rotate(camera.getTransform().getRotation().getRightVector(), -deltaTime);
					break;
				case SDLK_LEFT:
					camera.rotate(camera.getTransform().getRotation().getUpVector(), deltaTime);
					break;
				case SDLK_RIGHT:
					camera.rotate(camera.getTransform().getRotation().getUpVector(), -deltaTime);
					break;
				case SDLK_q:
					camera.rotate(camera.getTransform().getRotation().getForwardVector(), -deltaTime);
					break;
				case SDLK_e:
					camera.rotate(camera.getTransform().getRotation().getForwardVector(), deltaTime);
					break;
				}
			}
		}

		// Increment frame counter
		frames++;
	}

	// Quit SDL2 and the program after that
	SDL_Quit();
	return 0;
}