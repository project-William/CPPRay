#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vec3.h"
#include "quaternion.h"
#include "mat4.h"

class Transform
{
public:
	Transform(vec3 &position, quaternion &rotation, vec3 &scale);

	void update(float dt);

	vec3 *getPosition();
	quaternion *getRotation();
	vec3 *getScale();
private:
	vec3 *m_position;
	quaternion *m_rotation;
	vec3 *m_scale;
};

#endif
