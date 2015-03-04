#ifndef VEC3_H
#define VEC3_H

#include <string>

struct quaternion;

struct vec3
{
	float x, y, z;

	vec3(float x, float y, float z);
	vec3(float f);

	vec3 operator+=(const vec3 &other);
	vec3 operator+=(const float f);
	vec3 operator-=(const vec3 &other);
	vec3 operator-=(const float f);
	vec3 operator+(const vec3 &other);
	vec3 operator+(const float f);
	vec3 operator-(const vec3 &other);
	vec3 operator-(const float f);
	vec3 operator*=(const vec3 &other);
	vec3 operator*=(const float f);
	vec3 operator/=(const vec3 &other);
	vec3 operator/=(const float f);
	vec3 operator*(quaternion &q);
	vec3 operator*(const vec3 &other);
	vec3 operator*(const float f);
	vec3 operator/(const vec3 &other);
	vec3 operator/(const float f);

	float length();
	vec3 normalize();
	vec3 negate();
	std::string toString();

	static float dot(const vec3 &left, const vec3 &right);
	static vec3 cross(const vec3 &left, const vec3 &right);
	static vec3 reflect(vec3 &I, vec3 &N);
	static vec3 refract(vec3 &I, vec3 &N, float eta);

	inline bool operator==(const vec3 &other)
	{
		return x == other.x && y == other.y && z == other.z;
	}

	inline bool operator!=(const vec3 &other)
	{
		return !(*this == other);
	}
};

#endif