#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

using namespace math;

enum Refl_t
{
	DIFF, SPEC, REFR
};

class Material
{
public:
	Material(vec3 em = vec3(), vec3 re = vec3(), float ior = 1.52f, Refl_t refl_t = DIFF):m_emittance(em), m_reflectance(re), m_ior(ior), m_refl_t(refl_t) { }

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

	Refl_t getReflT() const
	{
		return m_refl_t;
	}

private:
	vec3 m_emittance;
	vec3 m_reflectance;
	float m_ior;
	Refl_t m_refl_t;
};

#endif
