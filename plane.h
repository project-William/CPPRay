#ifndef PLANE_H
#define PLANE_H

#include "vec3.h"
#include "ray.h"
#include "intersection.h"
#include "material.h"
#include "math.h"

class Plane
{
public:
    Plane(Transform transform = Transform(), Material material = Material()) : m_transform(transform), m_material(material) { }

    Intersection intersect(const Ray &r) const
    {
        vec3 P;
        float d, t;

        P = getPosition() - r.getOrigin();
        d = vec3::dot(getNormal(), r.getDirection());

        if (d > 0.0f)
            return invalidIntersection;

        t = vec3::dot(P, getNormal()) / d;

        if (t < EPSILON)
            return invalidIntersection;

        auto x = Intersection();
        x.setPosition(r.getOrigin() + r.getDirection() * t);
        x.setNormal(getNormal().normalize());
        x.setT(t);
        x.setMaterial(m_material);

        return x;
    }

    Transform getTransform() const
    {
        return m_transform;
    }

    vec3 getPosition() const
    {
        return m_transform.getPosition();
    }

    vec3 getNormal() const
    {
        quaternion q = m_transform.getRotation();
        return vec3(q.x, q.y, q.z);
    }
private:
    Transform m_transform;
    Material m_material;
protected:
};

#endif
