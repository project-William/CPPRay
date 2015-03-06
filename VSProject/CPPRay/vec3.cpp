#include "vec3.h"
#include "quaternion.h"

vec3::vec3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

vec3::vec3(float f)
{
	x = f;
	y = f;
	z = f;
}

vec3::vec3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

vec3 vec3::operator+=(const vec3 &other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

vec3 vec3::operator+=(const float f)
{
	x += f;
	y += f;
	z += f;
	return *this;
}

vec3 vec3::operator-=(const vec3 &other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

vec3 vec3::operator-=(const float f)
{
	x -= f;
	y -= f;
	z -= f;
	return *this;
}

vec3 vec3::operator*=(const vec3 &other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
	return *this;
}

vec3 vec3::operator*=(const float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

vec3 vec3::operator/=(const vec3 &other)
{
	x /= other.x;
	y /= other.y;
	z /= other.z;
	return *this;
}

vec3 vec3::operator/=(const float f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

vec3 vec3::operator+(const vec3 &other)
{
	return vec3(x + other.x, y + other.y, z + other.z);
}

vec3 vec3::operator+(const float f)
{
	return vec3(x + f, y + f, z + f);
}

vec3 vec3::operator-(const vec3 &other)
{
	return vec3(x - other.x, y - other.y, z - other.z);
}

vec3 vec3::operator-(const float f)
{
	return vec3(x - f, y - f, z - f);
}

vec3 vec3::operator*(quaternion &q)
{
	quaternion result = quaternion();

	result = q * *this * q.conjugate();

	return vec3(result.x, result.y, result.z);
}

vec3 vec3::operator*(const vec3 &other)
{
	return vec3(x * other.x, y * other.y, z * other.z);
}

vec3 vec3::operator*(const float f)
{
	return vec3(x * f, y * f, z * f);
}

vec3 vec3::operator/(const vec3 &other)
{
	return vec3(x / other.x, y / other.y, z / other.z);
}

vec3 vec3::operator/(const float f)
{
	return vec3(x / f, y / f, z / f);
}

float vec3::length()
{
	return std::sqrt(x * x + y * y + z * z);
}

vec3 vec3::normalize()
{
	float l = length();
	return vec3(x / l, y / l, z / l);
}

vec3 vec3::negate()
{
	return vec3(-x, -y, -z);
}

std::string vec3::toString()
{
	return "vec3[" + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + "]";
}

float vec3::dot(const vec3 &left, const vec3 &right)
{
	return left.x * right.x + left.y * right.y + left.z * right.z;
}

vec3 vec3::cross(const vec3 &left, const vec3 &right)
{
	return vec3(left.y * right.z - right.y * left.z, left.z * right.x - right.z * left.x, left.x * right.y - right.x * left.y);
}

vec3 vec3::reflect(vec3 &I, vec3 &N)
{
	return I - (N * dot(N, I) * 2.0f);
}

vec3 vec3::refract(vec3 &I, vec3 &N, float eta)
{
	float NdotI = dot(N, I);
	float k = 1.0f - eta * eta * (1.0f - NdotI * NdotI);

	if (k < 0.0f)
		return vec3();

	return I * eta - N * (eta * dot(N, I) + sqrt(k));
}