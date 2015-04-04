#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include "display.h"
#include "config.h"
#include "math.h"
#include "vec3.h"
#include "quaternion.h"
#include "mat4.h"
#include "light.h"
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
	void renderRT();
	void renderPT();
	void clearSamples();
private:
	vec3 raytrace(const Ray &r, int n);
	vec3 pathtrace(const Ray &r, int n, float weight);
	Intersection intersect(const Ray &r, float t);

	static const vec3 NULLCOLOR;
	static const vec3 AMBIENTCOLOR;

	int m_sampleAmount;
	vec3 *m_samples;
	Display *m_display;
	Camera *m_camera;
	std::vector<Light> m_lights;
	std::vector<Sphere> m_spheres;
	std::vector<Plane> m_planes;
};


#endif