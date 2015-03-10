#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

using namespace math;

class Material
{
public:
	Material(vec3 emittance, vec3 reflectance, float reflectivity, float refractivity, float ior);
	Material(vec3 reflectance, float reflectivity, float refractivity, float ior);
	Material(vec3 reflectance, float reflectivity, bool checkerboard);
	Material();

	vec3 Material::getEmittance() const
	{
		return m_emittance;
	}

	vec3 Material::getReflectance(const vec3 &v) const
	{
		if (!m_checkerboard)
			return m_reflectance;

		auto square = static_cast<int>(floor(v.x)) + static_cast<int>(floor(v.z));

		if (square % 2 == 0)
			return m_reflectance;
		else
			return vec3();
	}

	float Material::getReflectivity() const
	{
		return m_reflectivity;
	}

	float Material::getRefractivity() const
	{
		return m_refractivity;
	}

	float Material::getIOR() const
	{
		return m_ior;
	}
private:
	vec3 m_emittance;
	vec3 m_reflectance;
	float m_reflectivity;
	float m_refractivity;
	float m_ior;
	bool m_checkerboard;
};

#endif