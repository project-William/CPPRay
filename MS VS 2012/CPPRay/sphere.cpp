#include "sphere.h"
#include "vec3.h"
#include "ray.h"
#include "intersection.h"

Sphere::Sphere(vec3 position, float radius, Material material)
{
	m_position = position;
	m_radius = radius;
	m_material = material;
}

Sphere::Sphere()
{
	m_position = vec3();
	m_radius = 1.0f;
	m_material = Material();
}