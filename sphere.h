#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "ray.h"
#include "intersection.h"
#include "material.h"
#include "math.h"
#include "transform.h"

class Sphere
{
public:
    Sphere(Transform transform = Transform(), Material material = Material()) : m_transform(transform), m_material(material) { }

    Intersection intersect(const Ray &r) const
    {
        vec3 SP;
        float t, b, d;

        SP = getPosition() - r.getOrigin();
        b = vec3::dot(SP, r.getDirection());
        d = b * b - vec3::dot(SP, SP) + getRadius() * getRadius();

        if (d < 0.0f)
            return invalidIntersection;

        d = std::sqrt(d);
        t = (t = b - d) > EPSILON ? t : ((t = b + d) > EPSILON ? t : -1.0f);

        if (t == -1.0f)
            return invalidIntersection;

        auto x = Intersection();
        x.setPosition(r.getOrigin() + r.getDirection() * t);
        x.setNormal((x.getPosition() - getPosition()) / getRadius());
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

    float getRadius() const
    {
        return m_transform.getScale().x;
    }

    Material getMaterial() const
    {
        return m_material;
    }
private:
    Transform m_transform;
    Material m_material;
protected:
};

#endif
