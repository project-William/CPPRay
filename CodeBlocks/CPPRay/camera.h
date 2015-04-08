#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "quaternion.h"
#include "transform.h"

using namespace math;

class Camera
{
public:
	Camera(vec3 position, quaternion rotation, vec3 scale, float speed, float sensitivity);
	Camera();

	void move(const vec3 &direction, float dt);
	void rotate(const vec3 &axis, float dt);

	void setTransform(Transform t);
	void setPosition(vec3 position);
	void setRotation(quaternion rotation);
	void setScale(vec3 scale);

	Transform getTransform() const;
private:
	Transform m_transform;
	float m_speed;
	float m_sensitivity;
};

#endif
