#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "ray.h"
#include "intersection.h"
#include "material.h"

class Sphere
{
public:
	Sphere(vec3 position, float radius, Material material);
	Sphere();

	Intersection intersect(Ray r);

	void setPosition(vec3 v);
	void setRadius(float f);
private:
	vec3 m_position;
	float m_radius;
	Material m_material;
protected:
};

#endif