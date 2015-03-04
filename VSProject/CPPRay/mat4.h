#ifndef MAT4_H
#define MAT4_H

#include <string>
#include "vec3.h"

struct mat4
{
public:
	mat4();

	mat4 initIdentity();
	mat4 initTranslation(float x, float y, float z);
	mat4 initRotation();
	mat4 initScale(float x, float y, float z);

	mat4 operator*(const mat4 &other);
	mat4 operator*(const vec3 &other);

	float getValueAtIndex(int x, int y);
private:
	float *m;
};

#endif