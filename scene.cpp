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

    // Test building kd-tree
    std::vector<vec3> pointlist;
    for (size_t i = 0; i < m_triangles.size(); i++)
    {
        std::array<vertex, 3> vertices = m_triangles[i].getVertices();
        pointlist.push_back(vertices[0].p);
        pointlist.push_back(vertices[1].p);
        pointlist.push_back(vertices[2].p);
    }
    m_kdTree = accelerator::KDTree(pointlist, 3);

    std::cout << "Initialized a new Scene succesfully." << std::endl;
}
