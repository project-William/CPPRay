#include "scene.h"

Scene::Scene(int temp)
{
    m_dirLights.push_back(dirLight(quaternion().euler(1, 0, 0, 70.0f) * quaternion().euler(0, 1, 0, -22.5f), vec3(100, 100, 100)));

    Mesh cornellbox("cornellbox_0.obj", Material());
    cornellbox.calculateNormals();

    for (size_t i = 0; i < cornellbox.getTriangles().size(); i++)
    {
        m_triangles.push_back(cornellbox.getTriangles()[i]);
    }

    Material mat_metal(GLOS, vec3(), vec3(0.67f, 0.75f, 0.77f), 1.52f, 0.25f, 0.9f, 0.25f);
    m_spheres.push_back(Sphere(Transform(vec3(0.5f, -0.25f, 0), quaternion().identity(), vec3(0.25f, 0.25f, 0.25f)), mat_metal));

    std::cout << "Initialized a new Scene succesfully." << std::endl;
}
