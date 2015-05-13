#ifndef RAY_H
#define RAY_H

#include "vec3.h"

using namespace math;

class Ray
{
public:
    Ray(vec3 origin = vec3(), vec3 direction = vec3()) : m_origin(origin), m_direction(direction.normalize())
    {
        m_direction_inv = 1.0f / m_direction;
        sign[0] = (m_direction_inv.x < 0.0f);
        sign[1] = (m_direction_inv.y < 0.0f);
        sign[2] = (m_direction_inv.z < 0.0f);
    }

    void setOrigin(const vec3 &v)
    {
        m_origin = v;
    }

    void setDirection(const vec3 &v)
    {
        m_direction = v.normalize();
        m_direction_inv = 1.0f / m_direction;
        sign[0] = (m_direction_inv.x < 0.0f);
        sign[1] = (m_direction_inv.y < 0.0f);
        sign[2] = (m_direction_inv.z < 0.0f);
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

    int sign[3];
private:
    vec3 m_origin;
    vec3 m_direction;
    vec3 m_direction_inv;
};

#endif
