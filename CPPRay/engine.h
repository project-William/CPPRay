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
	void render();
	void renderMultithreaded(int width, int height, int xOffset, int yOffset, int id);
	vec3 raytrace(const Ray &r, int n);
	Intersection intersect(const Ray &r, float t);
private:
	static const vec3 NULLCOLOR;
	static const vec3 AMBIENTCOLOR;

	Display *m_display;
	Camera *m_camera;
	int m_workers;
	std::vector<Light> m_lights;
	std::vector<Sphere> m_spheres;
	std::vector<Plane> m_planes;
};


#endif