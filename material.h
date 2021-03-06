#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

using namespace math;

enum Refl_t
{
    DIFF = 0x01, SPEC = 0x02, REFR = 0x04, GLOS = 0x08
};

/*
inline Refl_t operator|(Refl_t left, Refl_t right)
{
    return static_cast<Refl_t>(static_cast<int>(left) | static_cast<int>(right));
}*/

/*  Material
    refl_t = Surface type
    emittance = Surface emission color
    reflectance = Surface reflection color
    ior = Index of refraction, glass = ~1.52f
    roughness = cook-torrance brdf term
    fresnel = cook-torrance brdf term
    density = cook-torrance brdf term
*/
class Material
{
public:
    Material(Refl_t refl_t = DIFF, vec3 em = vec3(), vec3 re = vec3(), float ior = 1.52f, float roughness = 0.25f, float fresnel = 1.0f, float density = 0.33f):
        m_refl_t(refl_t), m_emittance(em), m_reflectance(re), m_ior(ior), m_roughness(roughness), m_fresnel(fresnel), m_density(density) { }

    Refl_t getReflT() const
    {
        return m_refl_t;
    }

    vec3 getEmittance() const
    {
        return m_emittance;
    }

    vec3 getReflectance() const
    {
        return m_reflectance;
    }

    float getIOR() const
    {
        return m_ior;
    }

    float getRoughness() const
    {
        return m_roughness;
    }

    float getFresnel() const
    {
        return m_fresnel;
    }

    float getDensity() const
    {
        return m_density;
    }

    void setReflT(Refl_t t)
    {
        m_refl_t = t;
    }

    void setEmittance(vec3 emittance)
    {
        m_emittance = emittance;
    }

    void setReflectance(vec3 reflectance)
    {
        m_reflectance = reflectance;
    }

    void setIOR(float ior)
    {
        m_ior = ior;
    }

private:
    Refl_t m_refl_t;
    vec3 m_emittance;
    vec3 m_reflectance;
    float m_ior;
    float m_roughness;
    float m_fresnel;
    float m_density;
};

#endif
