#include "mesh.h"

Mesh::Mesh(std::string fileName, Material material, bool flatNormals)
{
    m_mdlFileName = fileName;
    m_material = material;

    if (loadObj() != 0)
    {
        std::cerr << "Mesh: Error loading " << m_mdlFileName << std::endl;
        std::exit(1);
    }

    if (flatNormals)
        calcFlatNormals();

    std::cout << "Mesh: " << m_mdlFileName << " has been loaded succesfully." << std::endl;
}

int Mesh::loadObj()
{
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<face> indices;
    std::map<std::string, Material> materials;
    std::string str_currentMaterial = "";

    std::ifstream file;
    std::string line;

    std::cout << "Mesh: " << "Attempting to load: " << m_mdlFileName << std::endl;

    file.open("res/" + m_mdlFileName);

    if (file.is_open())
    {
        while (file.good())
        {
            std::getline(file, line);

            if (line.substr(0, 7) == "mtllib ")
            {
                m_mtlFileName = line.substr(7);

                if (loadMTL(materials) != 0)
                {
                    std::cerr << "Mesh: Error loading " << m_mtlFileName << std::endl;
                    std::exit(1);
                }
            }
            else if (line.substr(0, 7) == "usemtl ")
            {
                str_currentMaterial = line.substr(7);
            }
            else if (line.substr(0, 2) == "v ")
            {
                std::istringstream s(line.substr(2));
                vec3 v;
                s >> v.x;
                s >> v.y;
                s >> v.z;
                vertices.push_back(v);
            }
            else if (line.substr(0, 3) == "vn ")
            {
                std::istringstream s(line.substr(3));
                vec3 n;
                s >> n.x;
                s >> n.y;
                s >> n.z;
                normals.push_back(n);
            }
            else if (line.substr(0, 2) == "f ")
            {
                if (line.find("//") == std::string::npos)
                {
                    std::istringstream s(line.substr(2));
                    face f;
                    s >> f.va;
                    s >> f.vb;
                    s >> f.vc;
                    f.va--;
                    f.vb--;
                    f.vc--;
                    f.material = str_currentMaterial;
                    indices.push_back(f);
                }
                else
                {
                    std::replace(line.begin(), line.end(), '/', ' ');
                    std::istringstream s(line.substr(2));
                    face f;
                    s >> f.va;
                    s >> f.na;
                    s >> f.vb;
                    s >> f.nb;
                    s >> f.vc;
                    s >> f.nc;
                    f.va--;
                    f.na--;
                    f.vb--;
                    f.nb--;
                    f.vc--;
                    f.nc--;
                    f.material = str_currentMaterial;
                    indices.push_back(f);
                }
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

    for (size_t i = 0; i < indices.size(); i++)
    {
        std::array<vertex, 3> verts;
        verts[0].p = vertices[indices[i].va];
        verts[1].p = vertices[indices[i].vb];
        verts[2].p = vertices[indices[i].vc];

        Material mtl = m_material;

        if (!str_currentMaterial.empty())
        {
            mtl = materials.at(indices[i].material);
        }

        if (normals.size() > 0)
        {
            verts[0].n = normals[indices[i].na];
            verts[1].n = normals[indices[i].nb];
            verts[2].n = normals[indices[i].nc];
        }

        m_triangles.push_back(Triangle(verts, mtl));
    }

    return 0;
}

int Mesh::loadMTL(std::map<std::string, Material> &materials)
{
    std::string str_currentMaterial;
    Material mat_currentMaterial;

    std::ifstream file;
    std::string line;

    std::cout << "Mesh: " << "Attempting to load: " << m_mtlFileName << std::endl;

    file.open("res/" + m_mtlFileName);

    if (file.is_open())
    {
        while (file.good())
        {
            std::getline(file, line);

            if (line.substr(0, 7) == "newmtl ")
            {
                str_currentMaterial = line.substr(7);
                mat_currentMaterial = Material();
                materials.insert(std::pair<std::string, Material>(str_currentMaterial, mat_currentMaterial));
            }
            else if (line.substr(0, 3) == "Kd ") // Diffuse color
            {
                std::istringstream s(line.substr(3));
                vec3 kd;
                s >> kd.x;
                s >> kd.y;
                s >> kd.z;
                materials.at(str_currentMaterial).setReflectance(kd);
            }
            else if (line.substr(0, 3) == "Ke ") // Emissive color (Blender doesn't want to export it, so it has to be added manually to the .mtl file)
            {
                std::istringstream s(line.substr(3));
                vec3 ke;
                s >> ke.x;
                s >> ke.y;
                s >> ke.z;
                materials.at(str_currentMaterial).setEmittance(ke);
            }
            else if (line.substr(0, 3) == "Ni ") // Index of refraction
            {
                std::istringstream s(line.substr(3));
                float ior;
                s >> ior;
                materials.at(str_currentMaterial).setIOR(ior);
            }
            else if (line.substr(0, 6) == "illum ") // Illumination mode
            {
                std::istringstream s(line.substr(6));
                unsigned int illum;
                s >> illum;

                if (illum == 0 || illum == 1) // Diffuse surfaces
                {
                    materials.at(str_currentMaterial).setReflT(DIFF);
                }
                if (illum == 4 || illum == 6 || illum == 7 || illum == 9) // Dielectric sufraces
                {
                    materials.at(str_currentMaterial).setReflT(REFR);
                }
                else if (illum == 3) // Perfectly specular surfaces
                {
                    materials.at(str_currentMaterial).setReflT(SPEC);
                }
                else if (illum == 5) // Glossy reflective surfaces
                {
                    materials.at(str_currentMaterial).setReflT(GLOS);
                }
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

    return 0;
}

void Mesh::translate(const vec3 v)
{
    m_transform.setPosition(m_transform.getPosition() + v);

    for (size_t i = 0; i < m_triangles.size(); i++)
    {
        for (size_t j = 0; j < m_triangles[i].getVertices().size(); j++)
        {
            m_triangles[i].getVertices()[j].p += m_transform.getPosition();
        }
    }
}

void Mesh::rotate(const vec3 axis, float theta)
{
    auto q = quaternion().euler(axis.x, axis.y, axis.z, theta);
    m_transform.setRotation((q * m_transform.getRotation()).normalize());

    for (size_t i = 0; i < m_triangles.size(); i++)
    {
        for (size_t j = 0; j < m_triangles[i].getVertices().size(); j++)
        {
            m_triangles[i].getVertices()[j].p = m_triangles[i].getVertices()[j].p * m_transform.getRotation();
        }
    }
}

void Mesh::scale(const vec3 v)
{
    m_transform.setScale(v);

    for (size_t i = 0; i < m_triangles.size(); i++)
    {
        for (size_t j = 0; j < m_triangles[i].getVertices().size(); j++)
        {
            m_triangles[i].getVertices()[j].p *= m_transform.getScale();
        }
    }
}

void Mesh::calcFlatNormals()
{
    for (size_t i = 0; i < m_triangles.size(); i++)
    {
        m_triangles[i].calcFlatNormals();
    }
}

std::vector<Triangle> Mesh::getTriangles()
{
    return m_triangles;
}
