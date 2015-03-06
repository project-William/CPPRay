#include "engine.h"

const vec3 Engine::NULLCOLOR = vec3();

Engine::Engine(Display *display, Camera *camera)
{
	m_display = display;

	// Scene main camera
	m_camera = camera;

	// Scene directional lights
	//m_lights.push_back(Light(vec3(0), vec3(1, 1, 1), vec3(1, 1, 1), vec3(0, 0, 1), 0.1f));

	// Scene point lights
	m_lights.push_back(Light(vec3(0, 2, 0), vec3(1, 1, 1), vec3(0, 0, 0.15f), 1.0f));
	m_lights.push_back(Light(vec3(-2, 0, -4), vec3(0.25f, 0.5f, 1), vec3(0, 0, 0.5f), 1.0f));

	// Scene materials
	Material mat_w_diffuse = Material(vec3(), vec3(1), 0, 0, 1);
	Material mat_r_diffuse = Material(vec3(), vec3(1, 0, 0), 0, 0, 1);
	Material mat_g_diffuse = Material(vec3(), vec3(0, 1, 0), 0, 0, 1);
	Material mat_b_diffuse = Material(vec3(), vec3(0, 0, 1), 0, 0, 1);
	Material mat_r_reflective = Material(vec3(), vec3(1, 0, 0), 1, 0, 1);
	Material mat_g_reflective = Material(vec3(), vec3(0, 1, 0), 1, 0, 1);
	Material mat_b_reflective = Material(vec3(), vec3(0, 0, 1), 1, 0, 1);
	Material mat_mirror = Material(vec3(), vec3(0), 1, 0, 1);
	Material mat_glass = Material(vec3(), vec3(0), 0, 1, 1.52f);

	// Scene spheres
	m_spheres.push_back(Sphere(vec3(0, 0, -5), 1, mat_w_diffuse));
	m_spheres.push_back(Sphere(vec3(-1, 0.75f, -4), 0.5f, mat_mirror));
	m_spheres.push_back(Sphere(vec3(1, -0.5f, -3), 0.5f, mat_g_reflective));
	m_spheres.push_back(Sphere(vec3(-2.5f, -0.5f, -5.5f), 0.5f, mat_r_reflective));

	// Scene planes
	m_planes.push_back(Plane(vec3(0, -1, 0), vec3(0, 1, 0), mat_w_diffuse));
}

Engine::~Engine()
{
	m_display = nullptr;
	m_camera = nullptr;

	std::cout << "Engine object has been destroyed succesfully." << std::endl;
}

void Engine::update(float dt)
{

}

void Engine::render()
{
	int w = m_display->getWidth();
	int h = m_display->getHeight();
	float ar = m_display->getAspectRatio();
	Ray r_primary = Ray(m_camera->getTransform().getPosition());
	quaternion &q = m_camera->getTransform().getRotation();
	quaternion q_inv = q.conjugate();

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			// Construct the ray's direction vector and aim it towards the virtual screen's pixel
			float x_norm = (x - w * 0.5f) / w * ar;
			float y_norm = (h * 0.5f - y) / h;
			vec3 v_norm = vec3(x_norm, y_norm, -1.0f);

			// Rotate the ray direction based on camera look direction
			quaternion w = quaternion(0, v_norm.x, v_norm.y, v_norm.z);
			quaternion r = q * w * q_inv;

			// Set the final direction of the rotated ray dir
			r_primary.setDirection(vec3(r.x, r.y, r.z));

			// Raytrace
			vec3 radiance = raytrace(r_primary, 0);

			// If something was hit, shade the pixel
			if (radiance != vec3(0))
				m_display->setPixel(x, y, radiance);
		}
	}
}

vec3 Engine::raytrace(Ray &r, int n)
{
	// Immediately return vec3(0) if recursion limit was hit
	if (n > RECURSION_MAX)
		return NULLCOLOR;

	// Initialize all raytracing data
	vec3 radiance = vec3();

	// Find the closest intersection
	Intersection xFinal = intersect(r, 1e6f);

	// Return black if no intersection happened
	if (xFinal == Intersection::invalidIntersection)
		return NULLCOLOR;

	// Initialize some variables that are not going to change
	vec3 V_vector = r.getDirection().negate();

	// Shade each hit surface agains each light source
	for (auto &l : m_lights)
	{
		float NdotL, NdotH, L_length, A;
		vec3 L_vector, H_vector;

		if (l.getType() == LIGHT_DIRECTIONAL)
		{
			L_vector = l.getDirection();
			L_length = 1e6f;
			H_vector = (V_vector + L_vector).normalize();
			A = 1.0f;
		}
		else if (l.getType() == LIGHT_POINT)
		{
			L_vector = l.getPosition() - xFinal.getPosition();
			L_length = L_vector.length();
			H_vector = (V_vector + L_vector).normalize();
			A = l.getAttenuation().x + l.getAttenuation().y * L_length + l.getAttenuation().z * L_length * L_length + 1e-3f;
		}
		else
		{
			continue;
		}

		// Trace shadows
		Ray r_shadow = Ray(xFinal.getPosition(), L_vector);
		Intersection xShadow = intersect(r_shadow, L_length);

		if (xShadow != Intersection::invalidIntersection)
			continue;

		// Basic lambertian shading for now
		NdotL = std::max(vec3::dot(xFinal.getNormal(), L_vector.normalize()), 0.0f);

		// Do not calculate shading if the color is going to be black
		if (NdotL <= 0.0f)
			continue;

		NdotH = std::max(vec3::dot(xFinal.getNormal(), H_vector), 0.0f);
		radiance += (xFinal.getMaterial().getReflectance() * l.getColor() * NdotL * l.getIntensity()) / A;
		radiance += std::pow(NdotH, 50) * l.getIntensity() / A;
	}

	// Reflect the ray if the hit surface is reflective
	if (xFinal.getMaterial().getReflectivity() > 0.0f)
	{
		Ray r_reflected = Ray(xFinal.getPosition(), vec3::reflect(r.getDirection(), xFinal.getNormal()));
		radiance += raytrace(r_reflected, n + 1);
	}

	// Refract the ray if the hit surface is refractive
	if (xFinal.getMaterial().getRefractivity() > 0.0f)
	{
		Ray r_refracted = Ray(xFinal.getPosition());
		vec3 &N = xFinal.getNormal();
		float NdotI = vec3::dot(r.getDirection(), N), ior, n1, n2, cos_t;

		if (NdotI > 0.0f)
		{
			n1 = r.getIOR();
			n2 = xFinal.getMaterial().getIOR();
			N = N.negate();
		}
		else
		{
			n1 = xFinal.getMaterial().getIOR();
			n2 = r.getIOR();
			NdotI = -NdotI;
		}

		ior = n2 / n1;
		cos_t = ior * ior * (1.0f - NdotI * NdotI);
		r_refracted = Ray(xFinal.getPosition(), vec3::refract(r.getDirection(), N, NdotI, ior, cos_t), 1.0f);
		radiance += raytrace(r_refracted, n + 1);
	}

	return radiance;
}

Intersection Engine::intersect(Ray &r, float t)
{
	Intersection xInit = Intersection::invalidIntersection;
	Intersection xFinal = Intersection::invalidIntersection;

	// Find the nearest intersection against a sphere if there's any
	for (auto &s : m_spheres)
	{
		xInit = s.intersect(r);

		if (xInit != Intersection::invalidIntersection && xInit.getT() < t)
		{
			xFinal = xInit;
			t = xFinal.getT();
		}
	}

	// Find the nearest intersection against a plane if there's any
	for (auto &p : m_planes)
	{
		xInit = p.intersect(r);

		if (xInit != Intersection::invalidIntersection && xInit.getT() < t)
		{
			xFinal = xInit;
			t = xFinal.getT();
		}
	}

	return xFinal;
}