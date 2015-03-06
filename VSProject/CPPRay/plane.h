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

	void setPosition(vec3 v);
	void setNormal(vec3 v);
	Intersection intersect(Ray r);
private:
	vec3 m_position;
	vec3 m_normal;
	Material m_material;
protected:
};

#endif