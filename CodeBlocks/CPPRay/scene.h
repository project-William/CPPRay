#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
    Scene()
    {
        //// SCENE DATA BELOW
        Material mat_light_w = Material(DIFF, vec3(25, 25, 25));
        Material mat_light_r = Material(DIFF, vec3(25, 0, 0));
        Material mat_diff_w = Material(DIFF, vec3(0, 0, 0), vec3(1, 1, 1));
        Material mat_diff_r = Material(DIFF, vec3(0, 0, 0), vec3(0.9f, 0.1f, 0.1f));
        Material mat_diff_g = Material(DIFF, vec3(0, 0, 0), vec3(0.1f, 0.9f, 0.1f));
        Material mat_diff_b = Material(DIFF, vec3(0, 0, 0), vec3(0.1f, 0.1f, 0.9f));
        Material mat_refl_w = Material(SPEC, vec3(0, 0, 0), vec3(1, 1, 1));
        Material mat_refr_w = Material(REFR, vec3(0, 0, 0), vec3(1, 1, 1), 1.52f);
        Material mat_glos_silver = Material(GLOS, vec3(0, 0, 0), vec3(0.75f, 0.75f, 0.75f), 1.0f, 0.20f, 1.0f, 0.33f);
        Material mat_glos_gold = Material(GLOS, vec3(0, 0, 0), vec3(0.75f, 0.66f, 0.2f), 1.0f, 0.16f, 1.0f, 0.4f);

        m_triangles.push_back(Triangle(vec3(-1, 3.99f, -5), vec3(1, 3.99f, -5), vec3(-1, 3.99f, -6), mat_light_w)); // Lamp
        m_triangles.push_back(Triangle(vec3(1, 3.99f, -5), vec3(-1, 3.99f, -6), vec3(1, 3.99f, -6), mat_light_w)); // Lamp

        m_spheres.push_back(Sphere(vec3(-1, 0.75f, -7), 0.75f, mat_diff_w));
        m_spheres.push_back(Sphere(vec3(1, 0.75f, -5), 0.75f, mat_refr_w));

        m_planes.push_back(Plane(vec3(0, 0, 0), vec3(0, 1, 0), mat_diff_w));
        m_planes.push_back(Plane(vec3(0, 4, 0), vec3(0, -1, 0), mat_diff_w));
        m_planes.push_back(Plane(vec3(2, 0, 0), vec3(-1, 0, 0), mat_diff_g));
        m_planes.push_back(Plane(vec3(-2, 0, 0), vec3(1, 0, 0), mat_diff_r));
        m_planes.push_back(Plane(vec3(0, 0, -8), vec3(0, 0, 1), mat_diff_w));
        m_planes.push_back(Plane(vec3(0, 0, 0), vec3(0, 0, -1), mat_diff_w));
    }

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
protected:
};

#endif
