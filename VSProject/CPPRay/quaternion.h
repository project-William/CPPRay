#ifndef QUATERNION_H
#define QUATERNION_H

#define PI 3.14159265359f

#include <string>

struct vec3;

struct quaternion
{
	float w, x, y, z;

	quaternion(float w, float x, float y, float z);
	quaternion();

	quaternion operator*(const quaternion &q);
	quaternion operator*(const vec3 &v);

	float length();
	quaternion normalize();
	quaternion conjugate();
	std::string toString();

	quaternion createFromAxisAngle(float x, float y, float z, float theta);
	quaternion identity();
	vec3 getForwardVector();
	vec3 getUpVector();
	vec3 getRightVector();
};

#endif
