#include "plane.h"
#include "vec3.h"
#include "ray.h"
#include "intersection.h"

Plane::Plane(vec3 position, vec3 normal, Material material)
{
	m_position = position;
	m_normal = normal;
	m_material = material;
}

Plane::Plane()
{
	m_position = vec3();
	m_normal = vec3(0, 1, 0);
	m_material = Material();
}