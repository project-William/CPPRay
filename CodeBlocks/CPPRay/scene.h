#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
    Scene()
    {
        //// SCENE DATA BELOW
        Material mat_light_w = Material(DIFF, vec3(3.33f, 3.33f, 3.33f));
        Material mat_diff_w = Material(DIFF, vec3(0, 0, 0), vec3(1, 1, 1));
        Material mat_diff_r = Material(DIFF, vec3(0, 0, 0), vec3(1, 0.1f, 0));
        Material mat_diff_g = Material(DIFF, vec3(0, 0, 0), vec3(0.25f, 1, 0));
        Material mat_diff_b = Material(DIFF, vec3(0, 0, 0), vec3(0, 0.25f, 1));
        Material mat_refl_w = Material(SPEC, vec3(0, 0, 0), vec3(1, 1, 1));
        Material mat_refr_w = Material(REFR, vec3(0, 0, 0), vec3(1, 1, 1), 1.52f);
        Material mat_glos_silver1 = Material(GLOS, vec3(0, 0, 0), vec3(0.75f, 0.75f, 0.75f), 1.0f, 0.75f, 1.0f, 0.33f);
        Material mat_glos_silver2 = Material(GLOS, vec3(0, 0, 0), vec3(0.75f, 0.75f, 0.75f), 1.0f, 0.5f, 1.0f, 0.33f);
        Material mat_glos_silver3 = Material(GLOS, vec3(0, 0, 0), vec3(0.75f, 0.75f, 0.75f), 1.0f, 0.25f, 1.0f, 0.33f);
        Material mat_glos_bronze1 = Material(GLOS, vec3(0, 0, 0), vec3(0.8f, 0.5f, 0.19f), 1.0f, 0.24f, 1.0f, 0.25f);
        Material mat_glos_bronze2 = Material(GLOS, vec3(0, 0, 0), vec3(0.8f, 0.5f, 0.19f), 1.0f, 0.16f, 1.0f, 0.25f);
        Material mat_glos_bronze3 = Material(GLOS, vec3(0, 0, 0), vec3(0.8f, 0.5f, 0.19f), 1.0f, 0.08f, 1.0f, 0.25f);

        //m_spheres.push_back(Sphere(vec3(0, 4.0f, -6), 0.75f, mat_light_w)); // Light
        m_spheres.push_back(Sphere(vec3(-1, 0.75f, -7), 0.75f, mat_glos_silver1));
        m_spheres.push_back(Sphere(vec3(0, 0.75f, -6), 0.75f, mat_glos_silver2));
        m_spheres.push_back(Sphere(vec3(1, 0.75f, -5), 0.75f, mat_refr_w));
        m_spheres.push_back(Sphere(vec3(-1.5f, 0.25f, -6), 0.25f, mat_refl_w));
        m_spheres.push_back(Sphere(vec3(-1.0f, 0.25f, -5.5f), 0.25f, mat_glos_bronze2));
        m_spheres.push_back(Sphere(vec3(-0.5f, 0.25f, -5), 0.25f, mat_glos_bronze3));

        m_planes.push_back(Plane(vec3(0, 0, 0), vec3(0, 1, 0), mat_diff_w));
        m_planes.push_back(Plane(vec3(0, 4, 0), vec3(0, -1, 0), mat_light_w));
        m_planes.push_back(Plane(vec3(2, 0, 0), vec3(-1, 0, 0), mat_diff_g));
        m_planes.push_back(Plane(vec3(-2, 0, 0), vec3(1, 0, 0), mat_diff_b));
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
private:
    std::vector<Sphere> m_spheres;
    std::vector<Plane> m_planes;
protected:
};

#endif
