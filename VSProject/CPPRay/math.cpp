#include "math.h"
#include "vec3.h"

math::math()
{
	std::cout << "math.cpp - You shouldn't instantiate this!" << std::endl;
}

float math::clamp(float f, float min, float max)
{
	return std::max(min, std::min(f, max));
}

vec3 math::clamp(vec3 v, float min, float max)
{
	float x = clamp(v.x, min, max);
	float y = clamp(v.y, min, max);
	float z = clamp(v.z, min, max);

	return vec3(x, y, z);
}

float math::smoothstep(float f, float min, float max)
{
	return clamp((f - min) / (max - min), 0.0f, 1.0f);
}

vec3 math::smoothstep(vec3 v, float min, float max)
{
	float x = smoothstep(v.x, min, max);
	float y = smoothstep(v.y, min, max);
	float z = smoothstep(v.z, min, max);

	return vec3(x, y, z);
}

float math::interpolate(float f, float edgea, float edgeb)
{
	return edgea + (edgeb - edgea) * clamp(f, 0.0f, 1.0f);
}

vec3 math::interpolate(vec3 v, float edgea, float edgeb)
{
	float x = interpolate(v.x, edgea, edgeb);
	float y = interpolate(v.y, edgea, edgeb);
	float z = interpolate(v.z, edgea, edgeb);

	return vec3(x, y, z);
}