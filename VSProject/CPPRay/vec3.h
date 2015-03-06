#ifndef VEC3_H
#define VEC3_H

#include <string>

struct vec3
{
	float x, y, z;

	vec3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	vec3(float f)
	{
		x = f;
		y = f;
		z = f;
	}

	vec3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	std::string toString()
	{
		return "vec3[" + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + "]";
	}

	bool operator==(const vec3 &other)
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator!=(const vec3 &other)
	{
		return !(*this == other);
	}

	vec3 operator+=(const vec3 &other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	vec3 operator+=(const float f)
	{
		x += f;
		y += f;
		z += f;
		return *this;
	}

	vec3 operator-=(const vec3 &other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	vec3 operator-=(const float f)
	{
		x -= f;
		y -= f;
		z -= f;
		return *this;
	}

	vec3 operator*=(const vec3 &other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	vec3 operator*=(const float f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	vec3 operator/=(const vec3 &other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	vec3 operator/=(const float f)
	{
		x /= f;
		y /= f;
		z /= f;
		return *this;
	}

	vec3 operator+(const vec3 &other)
	{
		return vec3(x + other.x, y + other.y, z + other.z);
	}

	vec3 operator+(const float f)
	{
		return vec3(x + f, y + f, z + f);
	}

	vec3 operator-(const vec3 &other)
	{
		return vec3(x - other.x, y - other.y, z - other.z);
	}

	vec3 operator-(const float f)
	{
		return vec3(x - f, y - f, z - f);
	}

	vec3 operator*(const vec3 &other)
	{
		return vec3(x * other.x, y * other.y, z * other.z);
	}

	vec3 operator*(const float f)
	{
		return vec3(x * f, y * f, z * f);
	}

	vec3 operator/(const vec3 &other)
	{
		return vec3(x / other.x, y / other.y, z / other.z);
	}

	vec3 operator/(const float f)
	{
		return vec3(x / f, y / f, z / f);
	}

	float length()
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	vec3 normalize()
	{
		float l = length();
		return vec3(x / l, y / l, z / l);
	}

	vec3 negate()
	{
		return vec3(-x, -y, -z);
	}

	static float dot(const vec3 &left, const vec3 &right)
	{
		return left.x * right.x + left.y * right.y + left.z * right.z;
	}

	static vec3 cross(const vec3 &left, const vec3 &right)
	{
		return vec3(left.y * right.z - right.y * left.z, left.z * right.x - right.z * left.x, left.x * right.y - right.x * left.y);
	}

	static vec3 reflect(vec3 &I, vec3 &N)
	{
		return I - (N * dot(N, I) * 2.0f);
	}

	static vec3 refract(vec3 &I, vec3 &N, float &NdotI, float &eta, float &cos_t)
	{
		cos_t = std::sqrt(1.0f - cos_t);

		if (cos_t < 1.0f)
			return I * eta - N * (eta * dot(N, I) + cos_t);
		else
			return reflect(I, N);
	}
};

#endif