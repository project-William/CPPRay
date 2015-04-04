#include "material.h"

Material::Material(vec3 emittance, vec3 reflectance, float reflectivity, float refractivity, float ior, float roughness, float fresnel, float density)
{
	m_emittance = emittance;
	m_reflectance = reflectance;
	m_reflectivity = reflectivity;
	m_refractivity = refractivity;
	m_ior = ior;
	m_roughness = roughness;
	m_fresnel = fresnel;
	m_density = density;
	m_checkerboard = false;
}

Material::Material(vec3 reflectance, float reflectivity, float refractivity, float ior, float roughness, float fresnel, float density)
{
	m_emittance = vec3();
	m_reflectance = reflectance;
	m_reflectivity = reflectivity;
	m_refractivity = refractivity;
	m_ior = ior;
	m_roughness = roughness;
	m_fresnel = fresnel;
	m_density = density;
	m_checkerboard = false;
}

Material::Material(vec3 reflectance, float reflectivity, float roughness, float fresnel, float density, bool checkerboard)
{
	m_emittance = vec3();
	m_reflectance = reflectance;
	m_reflectivity = reflectivity;
	m_refractivity = 0.0f;
	m_ior = 1.0f;
	m_roughness = roughness;
	m_fresnel = fresnel;
	m_density = density;
	m_checkerboard = checkerboard;
}

Material::Material()
{
	m_emittance = vec3(0.0f);
	m_reflectance = vec3(1.0f);
	m_reflectivity = 0.0f;
	m_refractivity = 0.0f;
	m_ior = 1.0f;
	m_roughness = 0.5f;
	m_fresnel = 1.0f;
	m_density = 0.95f;
	m_checkerboard = false;
}