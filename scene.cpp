#include "scene.h"

Scene::Scene()
{
    m_dirLights.push_back(dirLight(quaternion().euler(1, 0, 0, 110.0f) * quaternion().euler(0, 1, 0, -25.0f), vec3(25, 25, 25)));

    Material mat_metal(GLOS, vec3(), vec3(0.33f, 0.33f, 0.33f), 1.52f, 0.24f, 0.9f, 0.25f);
    Material mat_gold(GLOS, vec3(), vec3(0.9f, 0.75f, 0.1f), 1.52f, 0.05f, 0.9f, 0.25f);

    Mesh model1("dragon.obj", mat_metal, false);
    model1.scale(vec3(0.5f, 0.5f, 0.5f));
    model1.translate(vec3(4, 0, 0));
    for (size_t i = 0; i < model1.getTriangles().size(); i++)
    {
        m_triangles.push_back(model1.getTriangles()[i]);
    }

    Mesh model2("dragon.obj", mat_gold, false);
    model2.scale(vec3(0.5f, 0.5f, 0.5f));
    model2.translate(vec3(-4, 0, 0));
    for (size_t i = 0; i < model2.getTriangles().size(); i++)
    {
        m_triangles.push_back(model2.getTriangles()[i]);
    }

    //Material mat_glass(REFR, vec3(), vec3(0.9f, 0.33f, 0.33f), 1.52f);
    //Material mat_light(DIFF, vec3(16, 16, 16), vec3(0, 0, 0), 1.52f);
    //Material mat_diff(DIFF, vec3(), vec3(1, 1, 1));
    //m_spheres.push_back(Sphere(Transform(vec3(-0.3f, 1.5f, -0.3f), quaternion().identity(), vec3(0.25f, 0.25f, 0.25f)), mat_metal));
    //m_spheres.push_back(Sphere(Transform(vec3(0.25f, 0.9f, 0.25f), quaternion().identity(), vec3(0.25f, 0.25f, 0.25f)), mat_glass));
    //m_spheres.push_back(Sphere(Transform(vec3(-0.66f, 0.5f, 0.66f), quaternion().identity(), vec3(0.25f, 0.25f, 0.25f)), mat_light));

    // Test building the kd-tree
    m_kdTree = accelerator::KDTree(3);
    m_kdTree.init(m_triangles);

    std::cout << "Initialized a new Scene succesfully." << std::endl;
}
