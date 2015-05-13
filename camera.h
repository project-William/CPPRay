#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "quaternion.h"
#include "transform.h"

using namespace math;

enum camera_t
{
    ORTHOGRAPHIC, PERSPECTIVE
};

class Camera
{
public:
    Camera(camera_t projection = ORTHOGRAPHIC, Transform transform = Transform(), float speed = 2, float sensitivity = 100, float zoom = 0.1f) : m_projection(projection), m_transform(transform), m_speed(speed), m_sensitivity(sensitivity), m_zoom(zoom) { }

    void move(const vec3 &direction, float dt)
    {
        m_transform.setPosition(m_transform.getPosition() - direction * m_speed * dt);
    }

    void rotate(const vec3 &axis, float dt)
    {
        auto q = quaternion().euler(axis.x, axis.y, axis.z, m_sensitivity * dt);
        m_transform.setRotation((q * m_transform.getRotation()).normalize());
    }

    void setProjection(camera_t projection)
    {
        m_projection = projection;
    }

    void setTransform(Transform t)
    {
        m_transform = t;
    }

    void setPosition(vec3 position)
    {
        m_transform.setPosition(position);
    }

    void setRotation(quaternion rotation)
    {
        m_transform.setRotation(rotation);
    }

    void setScale(vec3 scale)
    {
        m_transform.setScale(scale);
    }

    void setZoom(float zoom)
    {
        m_zoom += zoom;
    }

    camera_t getProjection() const
    {
        return m_projection;
    }

    Transform getTransform() const
    {
        return m_transform;
    }

    float getZoom() const
    {
        return m_zoom;
    }

private:
    camera_t m_projection;
    Transform m_transform;
    float m_speed;
    float m_sensitivity;
    float m_zoom;
};

#endif
