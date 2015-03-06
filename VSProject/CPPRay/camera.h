#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "quaternion.h"
#include "mat4.h"
#include "transform.h"

class Camera
{
public:
	Camera(vec3 position, quaternion rotation, vec3 scale, float speed, float sensitivity);
	Camera();

	void move(vec3 &direction, float amount);
	void rotate(vec3 &axis, float theta);

	void setTransform(Transform t);
	void setPosition(vec3 position);
	void setRotation(quaternion rotation);
	void setScale(vec3 scale);

	Transform getTransform();
private:
	Transform m_transform;
	float m_speed;
	float m_sensitivity;
};

#endif