#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <array>
#include "vec3.h"
#include "quaternion.h"
#include "ray.h"
#include "intersection.h"
#include "material.h"
#include "math.h"

class Triangle
{
public:
    Triangle(std::array<vertex, 3> vertices = { }, Material material = Material()) : m_vertices(vertices), m_material(material) { }

    Intersection intersect(const Ray &r) const
    {
        vec3 P, Q, T;
        float d, inv_d, u, v, t, b0, b1, b2;

        const vec3 edge_a = m_vertices[1].p - m_vertices[0].p;
        const vec3 edge_b = m_vertices[2].p - m_vertices[0].p;

        P = vec3::cross(r.getDirection(), edge_b);
        d = vec3::dot(edge_a, P);

        if (d < EPSILON)
            return invalidIntersection;

        inv_d = 1.0f / d;
        T = r.getOrigin() - m_vertices[0].p;
        u = vec3::dot(T, P) * inv_d;

        if (u < 0.0f || u > 1.0f)
            return invalidIntersection;

        Q = vec3::cross(T, edge_a);
        v = vec3::dot(r.getDirection(), Q) * inv_d;

        if (v < 0.0f || u + v > 1.0f)
            return invalidIntersection;

        t = vec3::dot(edge_b, Q) * inv_d;

        if (t < EPSILON)
            return invalidIntersection;

        auto x = Intersection();
        x.setPosition(r.getOrigin() + r.getDirection() * t);
        x.setT(t);
        x.setMaterial(m_material);
        getBarycentric(edge_a, edge_b, x.getPosition(), b0, b1, b2);
        vec3 N1 = m_vertices[0].n;
        vec3 N2 = m_vertices[1].n;
        vec3 N3 = m_vertices[2].n;
        vec3 N = N1 + b1 * (N2 - N1) + b2 * (N3 - N1);
        N = N.normalize();
        x.setNormal(N);

        return x;
    }

    void calcFlatNormals()
    {
        for (auto &v : m_vertices)
        {
            v.n = vec3::cross(m_vertices[0].p - m_vertices[1].p, m_vertices[0].p - m_vertices[2].p).normalize();
        }
    }

    void getBarycentric(const vec3 &u, const vec3 &v, const vec3 &hit, float &b0, float &b1, float &b2) const
    {
        const vec3 w = hit - m_vertices[0].p;

        const vec3 vCrossW = vec3::cross(v, w);
        const vec3 uCrossW = vec3::cross(u, w);
        const vec3 uCrossV = vec3::cross(u, v);

        const float denom = uCrossV.length();
        const float r = vCrossW.length() / denom;
        const float t = uCrossW.length() / denom;

        b0 = 1.0f - r - t;
        b1 = r;
        b2 = t;
    }

    vec3 getCentroid() const
    {
        float x = (m_vertices[0].p.x + m_vertices[1].p.x + m_vertices[2].p.x) / 3.0f;
        float y = (m_vertices[0].p.y + m_vertices[1].p.y + m_vertices[2].p.y) / 3.0f;
        float z = (m_vertices[0].p.z + m_vertices[1].p.z + m_vertices[2].p.z) / 3.0f;

        return vec3(x, y, z);
    }

    vec3 getMaximum() const
    {
        vec3 result;

        result.x = std::max(m_vertices[0].p.x, std::max(m_vertices[1].p.x, m_vertices[2].p.x));
        result.y = std::max(m_vertices[0].p.y, std::max(m_vertices[1].p.y, m_vertices[2].p.y));
        result.z = std::max(m_vertices[0].p.z, std::max(m_vertices[1].p.z, m_vertices[2].p.z));

        return result;
    }

    vec3 getMinimum() const
    {
        vec3 result;

        result.x = std::min(m_vertices[0].p.x, std::min(m_vertices[1].p.x, m_vertices[2].p.x));
        result.y = std::min(m_vertices[0].p.y, std::min(m_vertices[1].p.y, m_vertices[2].p.y));
        result.z = std::min(m_vertices[0].p.z, std::min(m_vertices[1].p.z, m_vertices[2].p.z));

        return result;
    }

    std::array<vertex, 3> &getVertices()
    {
        return m_vertices;
    }

private:
    std::array<vertex, 3> m_vertices;
    Material m_material;
protected:
};

#endif // TRIANGLE_H
