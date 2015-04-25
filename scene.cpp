#include "scene.h"

Scene::Scene(int temp)
{
    //// SCENE DATA BELOW
    Material mat_light_w = Material(DIFF, vec3(3.5f, 3.5f, 3.5f));
    Material mat_light_c = Material(DIFF, vec3(5, 10, 10));
    Material mat_diff_w = Material(DIFF, vec3(0, 0, 0), vec3(1, 1, 1));
    Material mat_diff_r = Material(DIFF, vec3(0, 0, 0), vec3(0.9f, 0.1f, 0.1f));
    Material mat_diff_g = Material(DIFF, vec3(0, 0, 0), vec3(0.1f, 0.9f, 0.1f));
    Material mat_diff_b = Material(DIFF, vec3(0, 0, 0), vec3(0.1f, 0.1f, 0.9f));
    Material mat_diff_c = Material(DIFF, vec3(0, 0, 0), vec3(0.1f, 0.5f, 0.9f));
    Material mat_refl_w = Material(SPEC, vec3(0, 0, 0), vec3(1, 1, 1));
    Material mat_refr_w = Material(REFR, vec3(0, 0, 0), vec3(1, 1, 1), 1.52f);
    Material mat_glos_silver = Material(GLOS, vec3(0, 0, 0), vec3(0.75f, 0.75f, 0.75f), 1.0f, 0.05f, 0.9f, 0.5f);
    Material mat_glos_gold = Material(GLOS, vec3(0, 0, 0), vec3(0.75f, 0.66f, 0.2f), 1.0f, 0.05f, 0.9f, 0.5f);
    Material mat_glos_bronze = Material(GLOS, vec3(0, 0, 0), vec3(0.803f, 0.5f, 0.196f), 1.0f, 0.05f, 0.9f, 0.5f);

    //m_triangles.push_back(Triangle(vec3(-1, 3.99f, -5), vec3(1, 3.99f, -5), vec3(-1, 3.99f, -6), mat_light_w)); // Lamp
    //m_triangles.push_back(Triangle(vec3(1, 3.99f, -5), vec3(-1, 3.99f, -6), vec3(1, 3.99f, -6), mat_light_w)); // Lamp

    m_spheres.push_back(Sphere(Transform(vec3(-1, 0.25f, 2), quaternion().identity(), vec3(0.25f)), mat_refr_w));
    m_spheres.push_back(Sphere(Transform(vec3(0, 0.5f, 0), quaternion().identity(), vec3(0.5f)), mat_glos_silver));
    m_spheres.push_back(Sphere(Transform(vec3(1.25f, 0.5f, 0), quaternion().identity(), vec3(0.5f)), mat_glos_gold));
    m_spheres.push_back(Sphere(Transform(vec3(-1.25f, 0.5f, 0), quaternion().identity(), vec3(0.5f)), mat_glos_bronze));

    // Floor & Ceiling
    m_planes.push_back(Plane(Transform(vec3(0, 5, 0), quaternion(0, 0, -1, 0)), mat_light_w));
    m_planes.push_back(Plane(Transform(vec3(0, 0, 0), quaternion(0, 0, 1, 0)), mat_refr_w));
    m_planes.push_back(Plane(Transform(vec3(0, -0.1f, 0), quaternion(0, 0, 1, 0)), mat_diff_c));

    // Walls
    m_planes.push_back(Plane(Transform(vec3(2, 0, 0), quaternion(0, -1, 0, 0)), mat_diff_w));
    m_planes.push_back(Plane(Transform(vec3(-2, 0, 0), quaternion(0, 1, 0, 0)), mat_diff_b));
    m_planes.push_back(Plane(Transform(vec3(0, 0, -5), quaternion(0, 0, 0, 1)), mat_diff_w));
    m_planes.push_back(Plane(Transform(vec3(0, 0, 5), quaternion(0, 0, 0, -1)), mat_diff_w));


    Mesh cube1("res/cube.obj", mat_glos_bronze);
    cube1.scale(vec3(0.25f, 0.25f, 0.25f));
    cube1.rotate(vec3(0, 1, 0), 45.0f);
    cube1.rotate(vec3(1, 0, 1), 22.5f);
    cube1.translate(vec3(1, 0.33f, 2));
    cube1.calculateNormals();

    for (size_t i = 0; i < cube1.getTriangles().size(); i++)
    {
        m_triangles.push_back(cube1.getTriangles()[i]);
    }

    std::cout << "Initialized a new Scene succesfully." << std::endl;
}
