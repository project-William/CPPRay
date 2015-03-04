#include "transform.h"

Transform::Transform(vec3 &position, quaternion &rotation, vec3 &scale)
{
	*m_position = position;
	*m_rotation = rotation;
	*m_scale = scale;
}

void Transform::update(float dt)
{

}

vec3 *Transform::getPosition()
{
	return m_position;
}

quaternion *Transform::getRotation()
{
	return m_rotation;
}

vec3 *Transform::getScale()
{
	return m_scale;
}