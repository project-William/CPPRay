#include "camera.h"

Camera::Camera(vec3 position, quaternion rotation, vec3 scale, float speed, float sensitivity)
{
    m_transform = Transform(position, rotation, scale);
    m_speed = speed;
    m_sensitivity = sensitivity;
}

Camera::Camera()
{
    m_transform = Transform();
    m_speed = 16.0f;
    m_sensitivity = 32.0f;
}

void Camera::move(const vec3 &direction, float dt)
{
    m_transform.setPosition(m_transform.getPosition() - direction * m_speed * dt);
}

void Camera::rotate(const vec3 &axis, float dt)
{
    auto q = quaternion().euler(axis.x, axis.y, axis.z, m_sensitivity * dt);
    m_transform.setRotation((q * m_transform.getRotation()).normalize());
}

void Camera::setTransform(Transform t)
{
    m_transform = t;
}

void Camera::setPosition(vec3 position)
{
    m_transform.setPosition(position);
}

void Camera::setRotation(quaternion rotation)
{
    m_transform.setRotation(rotation);
}

void Camera::setScale(vec3 scale)
{
    m_transform.setScale(scale);
}

Transform Camera::getTransform() const
{
    return m_transform;
}
