#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include "sdl2/SDL.h"

class math;
struct vec3;

class Display
{
public:
	Display(char *title, int width, int height, int scale);
	~Display();

	int init();
	void render();
	void clear(Uint32 color);

	void setTitle(char *title);
	void setPixel(int x, int y, Uint32 color);
	void setPixel(int x, int y, vec3 color);

	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
	float getAspectRatio() { return (float) ((float) m_width / (float) m_height); }
	SDL_Window *getWindow() { return m_window; }
	SDL_Renderer *getRenderer() { return m_renderer; }
	SDL_Texture *getTexture() { return m_texture; }
private:
	char *m_title;
	int m_width;
	int m_height;
	int m_scale;
	Uint32 *m_pixels;
	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	SDL_Texture *m_texture;
};

#endif