#ifndef RAY_H
#define RAY_H

#include "vec3.h"

using namespace math;

class Ray
{
public:
    Ray(vec3 origin = vec3(), vec3 direction = vec3()) : m_origin(origin), m_direction(direction.normalize())
    {
        m_direction_inv.x = 1.0f / m_direction.x;
        m_direction_inv.y = 1.0f / m_direction.y;
        m_direction_inv.z = 1.0f / m_direction.z;
    }

    void setOrigin(const vec3 &v)
    {
        m_origin = v;
    }

    void setDirection(const vec3 &v)
    {
        m_direction = v.normalize();
        m_direction_inv.x = 1.0f / m_direction.x;
        m_direction_inv.y = 1.0f / m_direction.y;
        m_direction_inv.z = 1.0f / m_direction.z;
    }

    vec3 getOrigin() const
    {
        return m_origin;
    }

    vec3 getDirection() const
    {
        return m_direction;
    }

    vec3 getDirectionInv() const
    {
        return m_direction_inv;
    }

private:
    vec3 m_origin;
    vec3 m_direction;
    vec3 m_direction_inv;
};

#endif
