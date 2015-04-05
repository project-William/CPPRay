#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include "display.h"
#include "config.h"
#include "math.h"
#include "vec3.h"
#include "quaternion.h"
#include "mat4.h"
#include "sphere.h"
#include "plane.h"
#include "intersection.h"
#include "camera.h"

using namespace math;

class Engine
{
public:
	Engine(Display *display, Camera *camera);
	~Engine();

	void update(float dt);
	void renderPT();
	void clearSamples();
private:
	vec3 pathtrace(const Ray &r, int n, float weight, unsigned short *Xi);
	Intersection intersect(const Ray &r, float t);

	static const vec3 COLOR_NULL;
	static const vec3 COLOR_AMBI;

	int m_samplesppxl;
	vec3 *m_samples;
	Display *m_display;
	Camera *m_camera;
	std::vector<Sphere> m_spheres;
	std::vector<Plane> m_planes;
};


#endif