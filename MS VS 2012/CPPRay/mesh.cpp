#include "mesh.h"

Mesh::Mesh(char *fileName, Material material)
{
    m_fileName = fileName;
    m_material = material;

    if (loadObj() != 0)
    {
        std::cerr << "ObjMesh: Error loading " << m_fileName << std::endl;
        std::exit(1);
    }

    std::cout << "ObjMesh: " << m_fileName << " has been loaded succesfully." << std::endl;
}

int Mesh::loadObj()
{
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<face> indices;

    std::ifstream file;
    std::string line;

    file.open(m_fileName);

    if (file.is_open())
    {
        while (file.good())
        {
            std::getline(file, line);

            if (line.substr(0, 2) == "v ")
            {
                std::istringstream s(line.substr(2));
                vec3 v;
                s >> v.x;
                s >> v.y;
                s >> v.z;
                vertices.push_back(v);
            }
            else if (line.substr(0, 2) == "f ")
            {
                std::istringstream s(line.substr(2));
                face f;
                s >> f.a;
                s >> f.b;
                s >> f.c;
                f.a--;
                f.b--;
                f.c--;
                indices.push_back(f);
            }
            else
            {
                continue;
            }
        }
    }
    else
    {
        return 1;
    }

    assert((indices.size() % 3) == 0);

    for (size_t i = 0; i < indices.size(); i++)
    {
        vec3 v0 = vertices[indices[i].a];
        vec3 v1 = vertices[indices[i].b];
        vec3 v2 = vertices[indices[i].c];

        m_triangles.push_back(Triangle(v0, v1, v2, m_material));
    }

    return 0;
}
