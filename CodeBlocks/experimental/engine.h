#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <array>
#include <memory>
#include <iostream>
#include "display.h"
#include "config.h"
#include "math.h"
#include "vec3.h"
#include "quaternion.h"
#include "sphere.h"
#include "plane.h"
#include "intersection.h"
#include "camera.h"
#include "scene.h"

using namespace math;

struct sampler
{
    int samplesppx;
    vec3 *samples;
};

class Engine
{
public:
    Engine(Display *display, Camera *camera, int threads);
    ~Engine();

    void update(float dt);
    void render(int thread, int swidth, int sheight, int xoffset, int yoffset);
    vec3 pathtrace(const Ray &r, int n, unsigned short *Xi);
    void clearSamples();
    int getSamplesPPX();
private:
    Intersection intersect(const Ray &r, float t);

    static const vec3 COLOR_NULL;
    static const vec3 COLOR_AMBI;

    unsigned int m_threads;
    sampler *m_sampler;
    Scene m_scene;
    Display *m_display;
    Camera *m_camera;
};

#endif
