#ifndef MATH_H
#define MATH_H

#include <iostream>

struct vec3;

class math
{
public:
	static float clamp(float f, float min, float max);
	static vec3 clamp(vec3 v, float min, float max);
	static float smoothstep(float f, float min, float max);
	static vec3 smoothstep(vec3 v, float min, float max);
	static float interpolate(float f, float edgea, float edgeb);
	static vec3 interpolate(vec3 v, float edgea, float edgeb);
private:
	math();
protected:
};

#endif