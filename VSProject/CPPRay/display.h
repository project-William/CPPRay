#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include "sdl2/SDL.h"
#include "math.h"
#include "vec3.h"

class Display
{
public:
	Display(char *title, int width, int height, int scale);
	~Display();

	int init();
	void render();

	void clear(Uint32 color)
	{
		for (int i = 0; i < m_width * m_height; i++)
			m_pixels[i] = color;
	}

	void setPixel(int x, int y, Uint32 color)
	{
		if (x < 0 || x >= m_width || y < 0 || y >= m_height)
			return;

		m_pixels[x + y * m_width] = color;
	}

	void setPixel(int x, int y, vec3 &v)
	{
		if (x < 0 || x >= m_width || y < 0 || y >= m_height)
			return;

		v = math::clamp(v, 0.0f, 1.0f);

		Uint32 r = (Uint32) (v.x * 255.0f);
		Uint32 g = (Uint32) (v.y * 255.0f);
		Uint32 b = (Uint32) (v.z * 255.0f);
		Uint32 hex = ((r << 16) | (g << 8) | b);

		m_pixels[x + y * m_width] = hex;
	}

	void setTitle(char *title);

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