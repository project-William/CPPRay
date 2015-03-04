#ifndef ENGINE_H
#define ENGINE_H

#include "display.h"

class Engine
{
public:
	Engine(Display *display);
	~Engine();

	void update(float dt);
	void render();
private:
	Display *m_display;
};


#endif