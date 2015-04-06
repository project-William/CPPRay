#ifndef RAY_H
#define RAY_H

#include "vec3.h"

using namespace math;

class Ray
{
public:
	Ray(vec3 origin, vec3 direction, float ior);
	Ray(vec3 origin, vec3 direction);
	Ray(vec3 origin);
	Ray();

	void setOrigin(const vec3 &v);
	void setDirection(const vec3 &v);
	void setIOR(float f);
	vec3 getOrigin() const;
	vec3 getDirection() const;
	float getIOR() const;
private:
	vec3 m_origin;
	vec3 m_direction;
	float m_ior;
};

#endif