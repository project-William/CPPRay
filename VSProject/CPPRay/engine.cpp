#include "engine.h"

Engine::Engine(Display *display)
{
	m_display = display;
}

Engine::~Engine()
{
	m_display = nullptr;

	std::cout << "Engine object has been destroyed succesfully." << std::endl;
}

void Engine::update(float dt)
{

}

void Engine::render()
{

}