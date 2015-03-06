#ifndef LIGHT_H
#define LIGHT_H

#define LIGHT_NULL -1
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1

#include "math.h"
#include "vec3.h"

class Light
{
public:
	Light(vec3 position, vec3 direction, vec3 color, vec3 attenuation, float intensity);
	Light(vec3 position, vec3 color, vec3 attenuation, float intensity);
	Light();

	void setPosition(vec3 v);

	int getType();
	vec3 getPosition();
	vec3 getDirection();
	vec3 getColor();
	vec3 getAttenuation();
	float getIntensity();
private:
	vec3 m_position;
	vec3 m_direction;
	vec3 m_color;
	vec3 m_attenuation;
	float m_intensity;
};

#endif