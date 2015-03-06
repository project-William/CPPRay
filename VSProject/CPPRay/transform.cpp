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

mat4 Transform::getTransformation()
{
	mat4 translation = mat4().initTranslation(m_position.x, m_position.y, m_position.z);
	mat4 rotation = mat4().initRotation(m_rotation.w, m_rotation.x, m_rotation.y, m_rotation.z);
	mat4 scale = mat4().initScale(m_scale.x, m_scale.y, m_scale.z);

	return translation * rotation * scale;
}

vec3 Transform::getPosition()
{
	return m_position;
}

quaternion Transform::getRotation()
{
	return m_rotation;
}

vec3 Transform::getScale()
{
	return m_scale;
}