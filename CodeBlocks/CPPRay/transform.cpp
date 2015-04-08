#include "transform.h"

Transform::Transform(vec3 position, quaternion rotation, vec3 scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

Transform::Transform()
{
	m_position = vec3();
	m_rotation = quaternion().identity();
	m_scale = vec3();
}

void Transform::update(float dt)
{

}

void Transform::setPosition(vec3 v)
{
	m_position = v;
}

void Transform::setRotation(quaternion q)
{
	m_rotation = q;
}

void Transform::setScale(vec3 v)
{
	m_scale = v;
}

vec3 Transform::getPosition() const
{
	return m_position;
}

quaternion Transform::getRotation() const
{
	return m_rotation;
}

vec3 Transform::getScale() const
{
	return m_scale;
}
