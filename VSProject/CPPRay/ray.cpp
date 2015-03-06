#include "ray.h"
#include "vec3.h"

Ray::Ray(vec3 origin, vec3 direction)
{
	m_origin = origin;
	m_direction = direction.normalize();
}

Ray::Ray(vec3 origin)
{
	m_origin = origin;
	m_direction = vec3();
}

Ray::Ray()
{
	m_origin = vec3();
	m_direction = vec3();
}

void Ray::setOrigin(vec3 v)
{
	m_origin = v;
}

void Ray::setDirection(vec3 v)
{
	m_direction = v.normalize();
}

vec3 Ray::getOrigin()
{
	return m_origin;
}

vec3 Ray::getDirection()
{
	return m_direction;
}