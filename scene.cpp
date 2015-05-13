#include "scene.h"

Scene::Scene()
{
    //m_dirLights.push_back(dirLight(quaternion().euler(1, 0, 0, 110.0f) * quaternion().euler(0, 1, 0, -25.0f), vec3(16, 16, 16)));

    Mesh cornellbox("cornellbox.obj", Material(), false);
    cornellbox.translate(vec3(0, 0, 0));

    for (size_t i = 0; i < cornellbox.getTriangles().size(); i++)
    {
        m_triangles.push_back(cornellbox.getTriangles()[i]);
    }

    //Material mat_metal(GLOS, vec3(), vec3(0.67f, 0.75f, 0.77f), 1.52f, 0.25f, 0.9f, 0.5f);
    //Material mat_glass(REFR, vec3(), vec3(0.9f, 0.9f, 0.9f), 1.52f);
    //Material mat_diff(DIFF, vec3(), vec3(1, 1, 1));
    //m_spheres.push_back(Sphere(Transform(vec3(1, 0, 0), quaternion().identity(), vec3(1, 1, 1)), mat_diff));
    //m_spheres.push_back(Sphere(Transform(vec3(-0.5f, -0.75f, -0.25f), quaternion().identity(), vec3(0.25f, 0.25f, 0.25f)), mat_glass));

    // Test building the kd-tree
    m_kdTree = accelerator::KDTree(3);
    m_kdTree.init(m_triangles);

    std::cout << "Initialized a new Scene succesfully." << std::endl;
}
