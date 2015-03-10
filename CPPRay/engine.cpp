#include "engine.h"

const vec3 Engine::NULLCOLOR = vec3();
const vec3 Engine::AMBIENTCOLOR = vec3(0.1f);

Engine::Engine(Display *display, Camera *camera)
{
	// Display and camera
	m_display = display;
	m_camera = camera;

	// Multithreading
	m_workers = SDL_GetCPUCount();

	// Scene directional lights
	m_lights.push_back(Light(vec3(0), vec3(-1, 2, 1), vec3(1, 1, 1), vec3(0, 0, 1), 0.75f));

	// Scene point lights
	//m_lights.push_back(Light(vec3(0, 5, -3), vec3(1, 1, 1), vec3(0, 0, 0.1f), 2.0f));
	m_lights.push_back(Light(vec3(-10, 3, -3), vec3(1, 0, 1), vec3(0, 0, 0.1f), 1.0f));

	// Scene materials
	Material mat_w_checkerboard = Material(vec3(1), 0, true);
	Material mat_w_diffuse = Material(vec3(), vec3(1), 0, 0, 1);
	Material mat_r_diffuse = Material(vec3(), vec3(1, 0, 0), 0, 0, 1);
	Material mat_g_diffuse = Material(vec3(), vec3(0, 1, 0), 0, 0, 1);
	Material mat_b_diffuse = Material(vec3(), vec3(0, 0, 1), 0, 0, 1);
	Material mat_r_reflective = Material(vec3(), vec3(1, 0, 0), 0.75f, 0, 1);
	Material mat_g_reflective = Material(vec3(), vec3(0, 1, 0), 0.75f, 0, 1);
	Material mat_b_reflective = Material(vec3(), vec3(0, 0, 1), 0.75f, 0, 1);
	Material mat_c_reflective = Material(vec3(), vec3(0, 1, 1), 0.5f, 0, 1);
	Material mat_mirror = Material(vec3(), vec3(0), 1, 0, 1);
	Material mat_glass = Material(vec3(), vec3(0), 0, 1, 1.52f);

	// Scene spheres
	m_spheres.push_back(Sphere(vec3(-3, 1, -3), 1, mat_c_reflective));
	m_spheres.push_back(Sphere(vec3(-1, 0.5f, -3), 0.5f, mat_r_reflective));
	m_spheres.push_back(Sphere(vec3(-2, 0.25f, -1), 0.25f, mat_g_reflective));

	// Scene planes
	m_planes.push_back(Plane(vec3(0, 0, 0), vec3(0, 1, 0), mat_w_checkerboard));
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
	auto w = m_display->getWidth();
	auto h = m_display->getHeight();
	auto ar = m_display->getAspectRatio();
	auto r_primary = Ray(m_camera->getTransform().getPosition());
	auto &q = m_camera->getTransform().getRotation();
	auto &q_inv = q.conjugate();

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			// Pixel's color value
			auto &radiance = vec3();

			// Construct the ray's direction vector and aim it towards the virtual screen's pixel
			auto x_norm = (x - w * 0.5f) / w * ar;
			auto y_norm = (h * 0.5f - y) / h;
#if defined(SUPERSAMPLING)
			for (unsigned int i = 0; i < SUPERSAMPLINGLEVEL; i++)
			{
				// Random jitter per ray for supersampling
				auto x_rng = 0.5e-7f * (2 * (static_cast<float>(rand()) / static_cast<float>(1)) - 1);
				auto y_rng = 0.5e-7f * (2 * (static_cast<float>(rand()) / static_cast<float>(1)) - 1);

				// Apply the jitter to the ray dir
				auto v_norm = vec3(x_norm + x_rng, y_norm + y_rng, -1.0f);

				// Rotate the ray direction based on camera look direction
				auto w = quaternion(0, v_norm.x, v_norm.y, v_norm.z);
				auto r = q * w * q_inv;

				// Set the final direction of the rotated ray dir
				r_primary.setDirection(vec3(r.x, r.y, r.z));

				// Raytrace
				radiance += raytrace(r_primary, 0);
			}

			// If something was hit, shade the pixel
			if (radiance != vec3(0))
			{
				m_display->setPixel(x, y, radiance / SUPERSAMPLINGLEVEL);
			}
#else
			// Create the ray's direction vector based on normalized screenspace coordinates
			auto v_norm = vec3(x_norm, y_norm, -1.0f);

			// Rotate the ray direction based on camera look direction
			auto w = quaternion(0, v_norm.x, v_norm.y, v_norm.z);
			auto r = q * w * q_inv;

			// Set the final direction of the rotated ray dir
			r_primary.setDirection(vec3(r.x, r.y, r.z));

			// Raytrace
			radiance = raytrace(r_primary, 0);

			// If something was hit, shade the pixel
			if (radiance != vec3(0))
				m_display->setPixel(x, y, radiance);
#endif
		}
	}
}

vec3 Engine::raytrace(const Ray &r, int n)
{
	// Immediately return vec3(0) if recursion limit was hit
	if (n > RECURSION_MAX)
		return NULLCOLOR;

	// Initialize all raytracing data
	auto radiance = vec3();

	// Find the closest intersection
	Intersection xFinal = intersect(r, MAXDISTANCE);

	// Return black if no intersection happened
	if (xFinal == Intersection::invalidIntersection)
		return NULLCOLOR;

	// Initialize some variables that are not going to change
	auto &V_vector = r.getDirection().negate();

	// Add the ambient term always
	radiance += xFinal.getMaterial().getReflectance(xFinal.getPosition()) * AMBIENTCOLOR;

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
		auto r_shadow = Ray(xFinal.getPosition(), L_vector);
		auto xShadow = intersect(r_shadow, L_length);

		// Continue iterating to the next lightsource if shadow is to be cast
		if (xShadow != Intersection::invalidIntersection && xShadow.getMaterial().getRefractivity() <= 0.0f)
			continue;

		// Basic lambertian shading for now
		NdotL = std::max(vec3::dot(xFinal.getNormal(), L_vector.normalize()), 0.0f);

		// Do not calculate shading if the color is going to be black
		if (NdotL <= 0.0f)
			continue;

		NdotH = std::max(vec3::dot(xFinal.getNormal(), H_vector), 0.0f);
		radiance += (xFinal.getMaterial().getReflectance(xFinal.getPosition()) * l.getColor() * NdotL * l.getIntensity()) / A;
		radiance += std::pow(NdotH, 50) * l.getIntensity() / A;
	}

	// Reflect the ray if the hit surface is reflective
	if (xFinal.getMaterial().getReflectivity() > 0.0f)
	{
		auto r_reflected = Ray(xFinal.getPosition(), vec3::reflect(r.getDirection(), xFinal.getNormal()));
		radiance += raytrace(r_reflected, n + 1) * xFinal.getMaterial().getReflectivity();
	}

	// Refract the ray if the hit surface is refractive
	if (xFinal.getMaterial().getRefractivity() > 0.0f)
	{
		auto r_refracted = Ray(xFinal.getPosition());
		auto &N = xFinal.getNormal();
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
		radiance += raytrace(r_refracted, n + 1) * xFinal.getMaterial().getRefractivity();
	}

	return radiance;
}

Intersection Engine::intersect(const Ray &r, float t)
{
	auto xInit = Intersection::invalidIntersection;
	auto xFinal = Intersection::invalidIntersection;

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