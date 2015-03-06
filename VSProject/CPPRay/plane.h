#ifndef PLANE_H
#define PLANE_H

#include "vec3.h"
#include "ray.h"
#include "intersection.h"
#include "material.h"

class Plane
{
public:
	Plane(vec3 position, vec3 normal, Material material);
	Plane();

	Intersection intersect(Ray &r)
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

	void setPosition(vec3 v);
	void setNormal(vec3 v);
private:
	vec3 m_position;
	vec3 m_normal;
	Material m_material;
protected:
};

#endif