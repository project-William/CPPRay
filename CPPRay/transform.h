#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vec3.h"
#include "quaternion.h"
#include "mat4.h"

using namespace math;

class Transform
{
public:
	Transform(vec3 position, quaternion rotation, vec3 scale);
	Transform();

	void update(float dt);

	void setPosition(vec3 v);
	void setRotation(quaternion q);
	void setScale(vec3 v);

	mat4 getTransformation() const;
	vec3 getPosition() const;
	quaternion getRotation() const;
	vec3 getScale() const;
private:
	vec3 m_position;
	quaternion m_rotation;
	vec3 m_scale;
};

#endif
