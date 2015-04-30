#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include "assert.h"
#include "vec3.h"
#include "quaternion.h"
#include "transform.h"
#include "triangle.h"

using namespace math;

struct face
{
    int va, vb, vc;
    int na, nb, nc;
    std::string material;
};

class Mesh
{
public:
    Mesh(std::string fileName, Material material);

    void translate(const vec3 v)
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

    void rotate(const vec3 axis, float theta)
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

    void scale(const vec3 v)
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

    void calculateNormals()
    {
        for (size_t i = 0; i < m_triangles.size(); i++)
        {
            m_triangles[i].calculateNormals();
        }
    }

    std::vector<Triangle> getTriangles()
    {
        return m_triangles;
    }
private:
    // Very simple .obj model loader written by me
    // Only supports triangulated, non-textured faces atm
    int loadObj();
    int loadMTL(std::map<std::string, Material> &materials);

    std::string m_mdlFileName;
    std::string m_mtlFileName;
    Material m_material;
    Transform m_transform;
    std::vector<Triangle> m_triangles;
protected:
};

#endif // MESH_H
