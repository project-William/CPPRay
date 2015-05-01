#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "mesh.h"

struct dirLight
{
    dirLight(quaternion orientation = quaternion().identity(), vec3 color = vec3()) : orientation(orientation), color(color) { }
    quaternion orientation;
    vec3 color;
};

class Scene
{
public:
    Scene(int temp);
    Scene() { }

    std::vector<Sphere> getSceneSpheres()
    {
        return m_spheres;
    }

    std::vector<Plane> getScenePlanes()
    {
        return m_planes;
    }

    std::vector<Triangle> getSceneTriangles()
    {
        return m_triangles;
    }

    std::vector<dirLight> getSceneDirLights()
    {
        return m_dirLights;
    }

    std::vector<Sphere> getSceneLights()
    {
        std::vector<Sphere> result;
        for (auto &s : m_spheres)
        {
            if (s.getMaterial().getEmittance().length() > 0.0f)
                result.push_back(s);
        }
        return result;
    }
private:
    std::vector<Sphere> m_spheres;
    std::vector<Plane> m_planes;
    std::vector<Triangle> m_triangles;
    std::vector<dirLight> m_dirLights;
protected:
};

#endif
