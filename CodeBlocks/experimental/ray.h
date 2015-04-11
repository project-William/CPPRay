#ifndef RAY_H
#define RAY_H

#include "vec3.h"

using namespace math;

class Ray
{
public:
    Ray(vec3 origin = vec3(), vec3 direction = vec3()) : m_origin(origin), m_direction(direction.normalize()) { }

    void setOrigin(const vec3 &v)
    {
        m_origin = v;
    }

    void setDirection(const vec3 &v)
    {
        m_direction = v.normalize();
    }

    vec3 getOrigin() const
    {
        return m_origin;
    }

    vec3 getDirection() const
    {
        return m_direction;
    }
private:
    vec3 m_origin;
    vec3 m_direction;
};

#endif
