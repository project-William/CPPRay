#include "scene.h"

Scene::Scene(int temp)
{
    //m_dirLights.push_back(dirLight(quaternion().euler(1, 0, 0, 60.0f) * quaternion().euler(0, 1, 0, 20.0f), vec3(25, 25, 25)));

    Mesh cornellbox("cornellbox_1.obj", Material());
    cornellbox.calculateNormals();

    for (size_t i = 0; i < cornellbox.getTriangles().size(); i++)
    {
        m_triangles.push_back(cornellbox.getTriangles()[i]);
    }

    Material mat_metal(GLOS, vec3(), vec3(0.67f, 0.75f, 0.77f), 1.52f, 0.25f, 0.9f, 0.5f);
    Material mat_glass(REFR, vec3(), vec3(0.9f, 0.9f, 0.9f), 1.52f);
    m_spheres.push_back(Sphere(Transform(vec3(0.5f, -0.25f, 0), quaternion().identity(), vec3(0.25f, 0.25f, 0.25f)), mat_metal));
    m_spheres.push_back(Sphere(Transform(vec3(-0.5f, -0.75f, -0.25f), quaternion().identity(), vec3(0.25f, 0.25f, 0.25f)), mat_glass));

    std::cout << "Initialized a new Scene succesfully." << std::endl;
}
