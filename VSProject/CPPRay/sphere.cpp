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

Intersection Sphere::intersect(Ray r)
{
	vec3 SP;
	float t, b, d;

	SP = m_position - r.getOrigin();
	b = vec3::dot(SP, r.getDirection());
	d = b * b - vec3::dot(SP, SP) + m_radius * m_radius;

	if (d < 0.0f)
		return Intersection::invalidIntersection;
	
	d = std::sqrt(d);
	t = (t = b - d) > 1e-3 ? t : ((t = b + d) > 1e-3 ? t : -1.0f);

	if (t == -1.0f)
		return Intersection::invalidIntersection;

	Intersection x = Intersection();
	x.setPosition(r.getOrigin() + r.getDirection() * t);
	x.setNormal((x.getPosition() - m_position) / m_radius);
	x.setT(t);
	x.setMaterial(m_material);

	return x;
}

void Sphere::setPosition(vec3 v)
{
	m_position = v;
}

void Sphere::setRadius(float f)
{
	m_radius = f;
}