#ifndef VEC3_H
#define VEC3_H

#define PI 3.14159265359f

#include <string>
#include <cmath>
#include <random>

namespace math
{

// Forward declaration(s) for functions & objects that are in the 'math' namespace
static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<float>dis(0.0f, 1.0f);

static float pseudorand()
{
    return dis(gen);
}

struct vec3
{
    float x, y, z;

    vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f):x(x), y(y), z(z) { }

    std::string toString() const
    {
        char buffer[64];
        sprintf(buffer, "vec3[%.2f %.2f %.2f]", x, y, z);
        return std::string(buffer);
    }

    bool operator==(const vec3 &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const vec3 &other) const
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

    vec3 operator+(const vec3 &other) const
    {
        return vec3(x + other.x, y + other.y, z + other.z);
    }

    vec3 operator+(const float f) const
    {
        return vec3(x + f, y + f, z + f);
    }

    vec3 operator-(const vec3 &other) const
    {
        return vec3(x - other.x, y - other.y, z - other.z);
    }

    vec3 operator-(const float f) const
    {
        return vec3(x - f, y - f, z - f);
    }

    vec3 operator*(const vec3 &other) const
    {
        return vec3(x * other.x, y * other.y, z * other.z);
    }

    vec3 operator*(const float f) const
    {
        return vec3(x * f, y * f, z * f);
    }

    vec3 operator/(const vec3 &other) const
    {
        return vec3(x / other.x, y / other.y, z / other.z);
    }

    vec3 operator/(const float f) const
    {
        return vec3(x / f, y / f, z / f);
    }

    float length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    vec3 normalize() const
    {
        float l = length();
        return vec3(x / l, y / l, z / l);
    }

    vec3 negate() const
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

    static vec3 reflect(const vec3 &I, const vec3 &N)
    {
        return I - (N * dot(N, I) * 2.0f);
    }

    static vec3 sampleHemisphere(const vec3 &N)
    {
        float r1 = 2.0f * PI * pseudorand(); // Spherical coordinates
        float r2 = pseudorand();
        float r2s = std::sqrt(r2);
        vec3 w = N; // w = normal
        vec3 u = (cross((std::abs(w.x) > 0.1f ? vec3(0, 1) : vec3(1)), w)).normalize(); // u is perpendicular to w
        vec3 v = cross(w, u); // v is perpendicular to u and w
        return (u * std::cos(r1) * r2s + v * std::sin(r1) * r2s + w * std::sqrt(1.0f - r2)).normalize();
    }

    static vec3 sampleHemisphere(const vec3 &N, float scalar)
    {
        std::uniform_real_distribution<float>brdf_dis(0.0f, scalar);

        float r1 = 2.0f * PI * pseudorand(); // Spherical coordinates
        float r2;

        if (pseudorand() < 0.33f) // Importance sampling, should we bias the distribution or not?
            r2 = brdf_dis(gen);
        else
            r2 = pseudorand();

        float r2s = std::sqrt(r2);
        vec3 w = N; // w = normal
        vec3 u = (cross((std::abs(w.x) > 0.1f ? vec3(0, 1) : vec3(1)), w)).normalize(); // u is perpendicular to w
        vec3 v = cross(w, u); // v is perpendicular to u and w
        return (u * std::cos(r1) * r2s + v * std::sin(r1) * r2s + w * std::sqrt(1.0f - r2)).normalize();
    }

};

// Backwards operators
inline vec3 operator+(float f, const vec3 &v)
{
    return v + f;
}

inline vec3 operator-(float f, const vec3 &v)
{
    return v - f;
}

inline vec3 operator*(float f, const vec3 &v)
{
    return v * f;
}

inline vec3 operator/(float f, const vec3 &v)
{
    return vec3(f / v.x, f / v.y, f / v.z);
}

}

#endif
