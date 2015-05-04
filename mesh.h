#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <array>
#include <vector>
#include <map>
#include <string>
#include "assert.h"
#include "vec3.h"
#include "math.h"
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
    Mesh(std::string fileName, Material material, bool flatNormals);

    void translate(const vec3 v);
    void rotate(const vec3 axis, float theta);
    void scale(const vec3 v);
    void calcFlatNormals();
    std::vector<Triangle> getTriangles();
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
