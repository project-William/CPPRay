#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include "SDL.h"
#include "config.h"
#include "display.h"
#include "engine.h"
#include "input.h"

using namespace math;

int main(int argc, char** argv)
{
    // Disable cin/cout synchronization with C library buffers
    std::ios_base::sync_with_stdio(false);

    // Boolean which is used to check if the program is running or not
    bool running = true;

    // Initialize multithreading stuff
#if THREADS>1
    const unsigned int tcount = THREADS;
#else
    const unsigned int tcount = SDL_GetCPUCount();
#endif
    std::thread *threads = new std::thread[tcount];

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
    Display display("C++ Raytracer / Pathtracer", WIDTH, HEIGHT, SCALE);

    // Initialize the main camera
    Camera camera(vec3(0, 1, 0), quaternion().identity(), vec3(1), 2, 64);

    // Initialize the main engine object that handles the tracing
    Engine engine(&display, &camera, tcount);

    // Create SDL_Event object
    SDL_Event event;

    // Initialize rendering and updating related variables
    unsigned int frames = 0;
    unsigned int currentFrame = 0;
    unsigned int lastFrame = SDL_GetTicks();
    float deltaTime = 0;
    float frameTime = 0;

    while (running)
    {
        // Calculate delta-time & fps
        currentFrame = SDL_GetTicks();
        deltaTime = 0.1f * deltaTime + 0.9f * (float) (currentFrame - lastFrame) / 1000;
        lastFrame = currentFrame;
        frameTime = 1.0f / deltaTime;

        // Display info in console
        std::cout << "dt: " << deltaTime << "s" << " | FPS: " << frameTime << " | SPPX: " << engine.getSamplesPPX() << "\n";

        // Calculate rendering of the scene
        engine.update(deltaTime);
        display.clear(0x00000000);

        for (unsigned int i = 0; i < tcount; i++)
        {
            threads[i] = std::thread ([=,&engine] // capture i by value, engine by reference, makes sense now
            {
                engine.render(i, WIDTH, HEIGHT/tcount, 0, HEIGHT/tcount * i);
            }
                                     );
        }

        for (int j = 0; j < tcount; j++)
        {
            threads[j].join(); // Wait for all threads to finish by joining them to the main thread
        }

        display.render();

        // Handle input
        if (Input::g_keys[SDL_SCANCODE_W])
            camera.move(camera.getTransform().getRotation().getForwardVector(), deltaTime);
        else if (Input::g_keys[SDL_SCANCODE_S])
            camera.move(camera.getTransform().getRotation().getForwardVector(), -deltaTime);
        if (Input::g_keys[SDL_SCANCODE_A])
            camera.move(camera.getTransform().getRotation().getRightVector(), deltaTime);
        else if (Input::g_keys[SDL_SCANCODE_D])
            camera.move(camera.getTransform().getRotation().getRightVector(), -deltaTime);
        if (Input::g_keys[SDL_SCANCODE_R])
            camera.move(camera.getTransform().getRotation().getUpVector(), -deltaTime);
        else if (Input::g_keys[SDL_SCANCODE_F])
            camera.move(camera.getTransform().getRotation().getUpVector(), deltaTime);
        if (Input::g_keys[SDL_SCANCODE_UP])
            camera.rotate(camera.getTransform().getRotation().getRightVector(), deltaTime);
        else if (Input::g_keys[SDL_SCANCODE_DOWN])
            camera.rotate(camera.getTransform().getRotation().getRightVector(), -deltaTime);
        if (Input::g_keys[SDL_SCANCODE_LEFT])
            camera.rotate(camera.getTransform().getRotation().getUpVector(), deltaTime);
        else if (Input::g_keys[SDL_SCANCODE_RIGHT])
            camera.rotate(camera.getTransform().getRotation().getUpVector(), -deltaTime);
        if (Input::g_keys[SDL_SCANCODE_Q])
            camera.rotate(camera.getTransform().getRotation().getForwardVector(), -deltaTime);
        else if (Input::g_keys[SDL_SCANCODE_E])
            camera.rotate(camera.getTransform().getRotation().getForwardVector(), deltaTime);

        // Clear all samples if any key is pressed
        for (unsigned int i = 0; i < sizeof(Input::g_keys) / sizeof(*Input::g_keys); i++)
            if (Input::g_keys[i])
                engine.clearSamples();

        // Process SDL events
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
            {
                auto id = event.key.keysym.scancode;
                if (id > 0 && id < sizeof(Input::g_keys) / sizeof(*Input::g_keys))
                {
                    Input::g_keys[id] = true;
                }
            }
            break;
            case SDL_KEYUP:
            {
                auto id = event.key.keysym.scancode;
                if (id > 0 && id < sizeof(Input::g_keys) / sizeof(*Input::g_keys))
                {
                    Input::g_keys[id] = false;
                }
            }
            break;
            }
        }

        // Increment frame counter
        frames++;
    }

    // Free allocated memory
    delete[] threads;

    // Quit SDL2 and the program after that
    SDL_Quit();
    return 0;
}
