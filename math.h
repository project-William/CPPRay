#ifndef MATH_H
#define MATH_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include "vec3.h"

namespace math
{

#define EPSILON 1e-3f
#define PI 3.14159265359f
#define PI_1 1.0f / PI

struct vertex
{
    vec3 p;
    vec3 n;
};

static vec3 pow(const vec3 &v, float f)
{
    auto x = std::pow(v.x, f);
    auto y = std::pow(v.y, f);
    auto z = std::pow(v.z, f);

    return vec3(x, y, z);
}

static float clamp(float f, float min, float max)
{
    return std::max(min, std::min(f, max));
}

static vec3 clamp(const vec3 &v, float min, float max)
{
    auto x = clamp(v.x, min, max);
    auto y = clamp(v.y, min, max);
    auto z = clamp(v.z, min, max);

    return vec3(x, y, z);
}

static float smoothstep(float f, float min, float max)
{
    return clamp((f - min) / (max - min), 0.0f, 1.0f);
}

static vec3 smoothstep(const vec3 &v, float min, float max)
{
    auto x = smoothstep(v.x, min, max);
    auto y = smoothstep(v.y, min, max);
    auto z = smoothstep(v.z, min, max);

    return vec3(x, y, z);
}

static float interpolate(float f, float edgea, float edgeb)
{
    return edgea + (edgeb - edgea) * clamp(f, 0.0f, 1.0f);
}

static vec3 interpolate(const vec3 &v, float edgeax, float edgebx, float edgeay, float edgeby, float edgeaz, float edgebz)
{
    auto x = interpolate(v.x, edgeax, edgebx);
    auto y = interpolate(v.y, edgeay, edgeby);
    auto z = interpolate(v.z, edgeaz, edgebz);

    return vec3(x, y, z);
}

}

#endif
