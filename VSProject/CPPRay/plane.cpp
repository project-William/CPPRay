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

void Plane::setPosition(vec3 v)
{
	m_position = v;
}

void Plane::setNormal(vec3 v)
{
	m_normal = v;
}

Intersection Plane::intersect(Ray r)
{
	vec3 P;
	float d, t;

	P = m_position - r.getOrigin();
	d = vec3::dot(m_normal, r.getDirection());

	if (d > 0.0f)
		return Intersection::invalidIntersection;

	t = vec3::dot(P, m_normal) / d;

	if (t < 1e-3)
		return Intersection::invalidIntersection;

	Intersection x = Intersection();
	x.setPosition(r.getOrigin() + r.getDirection() * t);
	x.setNormal(m_normal);
	x.setT(t);
	x.setMaterial(m_material);

	return x;
}