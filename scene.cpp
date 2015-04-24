#include "scene.h"

Scene::Scene(int temp)
{
    //// SCENE DATA BELOW
    Material mat_light_w = Material(DIFF, vec3(3, 3, 3));
    Material mat_light_c = Material(DIFF, vec3(5, 10, 10));
    Material mat_diff_w = Material(DIFF, vec3(0, 0, 0), vec3(1, 1, 1));
    Material mat_diff_r = Material(DIFF, vec3(0, 0, 0), vec3(0.9f, 0.1f, 0.1f));
    Material mat_diff_g = Material(DIFF, vec3(0, 0, 0), vec3(0.1f, 0.9f, 0.1f));
    Material mat_diff_b = Material(DIFF, vec3(0, 0, 0), vec3(0.1f, 0.1f, 0.9f));
    Material mat_diff_c = Material(DIFF, vec3(0, 0, 0), vec3(0.1f, 0.5f, 0.9f));
    Material mat_refl_w = Material(SPEC, vec3(0, 0, 0), vec3(1, 1, 1));
    Material mat_refr_w = Material(REFR, vec3(0, 0, 0), vec3(1, 1, 1), 1.52f);
    Material mat_glos_silver = Material(GLOS, vec3(0, 0, 0), vec3(0.75f, 0.75f, 0.75f), 1.0f, 0.1f, 1.0f, 0.25f);
    Material mat_glos_gold = Material(GLOS, vec3(0, 0, 0), vec3(0.75f, 0.66f, 0.2f), 1.0f, 0.16f, 1.0f, 0.25f);
    Material mat_glos_floor = Material(GLOS, vec3(0, 0, 0), vec3(0.9f, 0.9f, 0.9f), 1.0f, 0.16f, 1.0f, 0.75f);

    //m_triangles.push_back(Triangle(vec3(-1, 3.99f, -5), vec3(1, 3.99f, -5), vec3(-1, 3.99f, -6), mat_light_w)); // Lamp
    //m_triangles.push_back(Triangle(vec3(1, 3.99f, -5), vec3(-1, 3.99f, -6), vec3(1, 3.99f, -6), mat_light_w)); // Lamp

    m_spheres.push_back(Sphere(vec3(-1, 0.66f, -5), 0.5f, mat_refr_w));
    m_spheres.push_back(Sphere(vec3(1.25f, 2.5f, -6), 0.5f, mat_glos_gold));
    m_spheres.push_back(Sphere(vec3(0.75f, 1.5f, -5.25f), 0.5f, mat_glos_silver));

    m_planes.push_back(Plane(vec3(0, 0, 0), vec3(0, 1, 0), mat_diff_w));
    m_planes.push_back(Plane(vec3(0, 4, 0), vec3(0, -1, 0), mat_light_w));
    m_planes.push_back(Plane(vec3(2, 0, 0), vec3(-1, 0, 0), mat_diff_g));
    m_planes.push_back(Plane(vec3(-2, 0, 0), vec3(1, 0, 0), mat_diff_b));
    m_planes.push_back(Plane(vec3(0, 0, -8), vec3(0, 0, 1), mat_diff_w));
    m_planes.push_back(Plane(vec3(0, 0, 0), vec3(0, 0, -1), mat_diff_w));

    Mesh cube1("res/cube.obj", mat_diff_w);
    cube1.rotate(vec3(0, 1, 0), 45.0f);
    cube1.scale(vec3(0.5f, 0.5f, 0.5f));
    cube1.translate(vec3(0.75f, 0.5f, -5));
    cube1.recalcNormals();

    Mesh cube2("res/cube.obj", mat_diff_w);
    cube2.rotate(vec3(0, 1, 0), -45.0f);
    cube2.scale(vec3(0.5f, 0.75f, 0.5f));
    cube2.translate(vec3(-0.75f, 2, -7));
    cube2.recalcNormals();

    Mesh torusknot("res/torusknot.obj", mat_glos_gold);
    torusknot.rotate(vec3(0, 1, 0), 22.5f);
    torusknot.scale(vec3(0.5f, 0.5f, 0.5f));
    torusknot.translate(vec3(0, 1, -5));

    for (size_t i = 0; i < cube1.getTriangles().size(); i++)
    {
        m_triangles.push_back(cube1.getTriangles()[i]);
        m_triangles.push_back(cube2.getTriangles()[i]);
    }

    std::cout << "Initialized a new Scene succesfully." << std::endl;
}
