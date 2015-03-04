#include "quaternion.h"

quaternion::quaternion(float w, float x, float y, float z)
{
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

quaternion::quaternion()
{
	w = 0.0f;
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

quaternion quaternion::operator*(const quaternion &q)
{
	quaternion result = quaternion();

	result.w = w * q.w - x * q.x - y * q.y - z * q.z;
	result.x = w * q.x + x * q.w + y * q.z - z * q.y;
	result.y = w * q.y - x * q.z + y * q.w + z * q.x;
	result.z = w * q.z + x * q.y - y * q.x + z * q.w;

	return result;
}

quaternion quaternion::operator*(const vec3 &v)
{
	quaternion result = quaternion();

	result.w = -x * v.x - y * v.y - z * v.z;
	result.x = w * v.x + y * v.z - z * v.y;
	result.y = w * v.y + z * v.x - x * v.z;
	result.z = w * v.z + x * v.y - y * v.x;

	return result;
}

float quaternion::length()
{
	return std::sqrt(w * w + x * x + y * y + z * z);
}

quaternion quaternion::normalize()
{
	float l = length();
	return quaternion(w / l, x / l, y / l, z / l);
}

quaternion quaternion::conjugate()
{
	return quaternion(w, -x, -y, -z);
}

std::string quaternion::toString()
{
	return "quaternion[" + std::to_string(w) + " " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + "]";
}

quaternion quaternion::createFromAxisAngle(float x, float y, float z, float theta)
{
	theta = theta * PI / 180.0f;

	this->w = std::cos(theta / 2.0f);
	this->x = x * std::sin(theta / 2.0f);
	this->y = y * std::sin(theta / 2.0f);
	this->z = z * std::sin(theta / 2.0f);

	return *this;
}

quaternion quaternion::initIdentity()
{
	w = 1.0f;
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;

	return *this;
}