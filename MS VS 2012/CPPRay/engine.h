#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <memory>
#include "display.h"
#include "config.h"
#include "math.h"
#include "vec3.h"
#include "quaternion.h"
#include "mat4.h"
#include "sphere.h"
#include "plane.h"
#include <iostream>
#include "intersection.h"
#include "camera.h"
#include "scene.h"

using namespace math;

class Engine
{
public:
	Engine(Display *display, Camera *camera);
	~Engine();

	void update(float dt);
	void renderPT(int swidth, int sheight, int xoffset, int yoffset);
	vec3 pathtrace(const Ray &r, int n, float weight, unsigned short *Xi);
	void clearSamples();
	int getSamplesPPX();
private:
	Intersection intersect(const Ray &r, float t);

	static const vec3 COLOR_NULL;
	static const vec3 COLOR_AMBI;

	int m_samplesppx;
	vec3 *m_samples;
	Scene m_scene;
	Display *m_display;
	Camera *m_camera;
};


#endif