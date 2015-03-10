#include "material.h"

Material::Material(vec3 emittance, vec3 reflectance, float reflectivity, float refractivity, float ior)
{
	m_emittance = emittance;
	m_reflectance = reflectance;
	m_reflectivity = reflectivity;
	m_refractivity = refractivity;
	m_ior = ior;
	m_checkerboard = false;
}

Material::Material(vec3 reflectance, float reflectivity, float refractivity, float ior)
{
	m_emittance = vec3();
	m_reflectance = reflectance;
	m_reflectivity = reflectivity;
	m_refractivity = refractivity;
	m_ior = ior;
	m_checkerboard = false;
}

Material::Material(vec3 reflectance, float reflectivity, bool checkerboard)
{
	m_emittance = vec3();
	m_reflectance = reflectance;
	m_reflectivity = reflectivity;
	m_refractivity = 0.0f;
	m_ior = 1.0f;
	m_checkerboard = checkerboard;
}

Material::Material()
{
	m_emittance = vec3();
	m_reflectance = vec3(1.0f);
	m_reflectivity = 0.0f;
	m_refractivity = 0.0f;
	m_ior = 1.0f;
	m_checkerboard = false;
}