#include "light.h"

Light::Light(vec3 position, vec3 direction, vec3 color, vec3 attenuation, float intensity)
{
	m_position = position;
	m_direction = direction.normalize();
	m_color = color;
	m_attenuation = attenuation;
	m_intensity = intensity;
}

Light::Light(vec3 position, vec3 color, vec3 attenuation, float intensity)
{
	m_position = position;
	m_direction = vec3();
	m_color = color;
	m_attenuation = attenuation;
	m_intensity = intensity;
}

Light::Light()
{
	m_position = vec3();
	m_direction = vec3();
	m_color = vec3(1);
	m_attenuation = vec3(0, 0, 1);
	m_intensity = 1;
}

void Light::setPosition(vec3 v)
{
	m_position = v;
}

int Light::getType()
{
	if (m_direction != vec3())
		return LIGHT_DIRECTIONAL;
	
	return LIGHT_POINT;
}

vec3 Light::getPosition()
{
	return m_position;
}

vec3 Light::getDirection()
{
	return m_direction;
}

vec3 Light::getColor()
{
	return m_color;
}

vec3 Light::getAttenuation()
{
	return m_attenuation;
}

float Light::getIntensity()
{
	return m_intensity;
}