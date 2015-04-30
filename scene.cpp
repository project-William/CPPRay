#include "scene.h"

Scene::Scene(int temp)
{
    Mesh cornellbox("cornellbox.obj", Material());
    cornellbox.calculateNormals();

    for (size_t i = 0; i < cornellbox.getTriangles().size(); i++)
    {
        m_triangles.push_back(cornellbox.getTriangles()[i]);
    }

    std::cout << "Initialized a new Scene succesfully." << std::endl;
}
