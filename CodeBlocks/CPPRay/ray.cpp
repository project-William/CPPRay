#include "ray.h"
#include "vec3.h"

Ray::Ray(vec3 origin, vec3 direction, float ior)
{
	m_origin = origin;
	m_direction = direction.normalize();
	m_ior = ior;
}

Ray::Ray(vec3 origin, vec3 direction)
{
	m_origin = origin;
	m_direction = direction.normalize();
	m_ior = 1.0f;
}

Ray::Ray(vec3 origin)
{
	m_origin = origin;
	m_direction = vec3();
	m_ior = 1.0f;
}

Ray::Ray()
{
	m_origin = vec3();
	m_direction = vec3();
	m_ior = 1.0f;
}

void Ray::setOrigin(const vec3 &v)
{
	m_origin = v;
}

void Ray::setDirection(const vec3 &v)
{
	m_direction = v.normalize();
}

void Ray::setIOR(float f)
{
	m_ior = f;
}

vec3 Ray::getOrigin() const
{
	return m_origin;
}

vec3 Ray::getDirection() const
{
	return m_direction;
}

float Ray::getIOR() const
{
	return m_ior;
}