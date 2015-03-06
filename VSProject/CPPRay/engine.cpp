#include "engine.h"

Engine::Engine(Display *display, Camera *camera)
{
	m_display = display;

	// Scene main camera
	m_camera = camera;

	// Scene directional lights
	//m_lights.push_back(Light(vec3(0), vec3(1, 1, 1), vec3(1, 1, 1), vec3(0, 0, 1), 1.0f));

	// Scene point lights
	m_lights.push_back(Light(vec3(2, 1, 0), vec3(1, 1, 1), vec3(0, 0, 0.25f), 1.0f));
	m_lights.push_back(Light(vec3(-1, 1, 0), vec3(0.1f, 0.5, 1), vec3(0, 0, 0.5f), 1.0f));

	// Scene materials
	Material mat_w_diffuse = Material(vec3(), vec3(1), 0, 0, 1);
	Material mat_r_diffuse = Material(vec3(), vec3(1, 0, 0), 0, 0, 1);
	Material mat_g_diffuse = Material(vec3(), vec3(0, 1, 0), 0, 0, 1);
	Material mat_b_diffuse = Material(vec3(), vec3(0, 0, 1), 0, 0, 1);

	// Scene spheres
	m_spheres.push_back(Sphere(vec3(0, 0, -5), 1, mat_r_diffuse));
	m_spheres.push_back(Sphere(vec3(-1, 0.75f, -4), 0.5f, mat_g_diffuse));
	m_spheres.push_back(Sphere(vec3(0.75f, -0.25f, -3), 0.5f, mat_b_diffuse));

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
	float t = SDL_GetTicks() / 1000.0f;
	m_spheres[1].setPosition(vec3(std::cos(t), std::sin(t), -4));
	m_lights[1].setPosition(vec3(std::cos(t * 0.75f), 1, -3));
}

void Engine::render()
{
	int w = m_display->getWidth();
	int h = m_display->getHeight();
	float ar = m_display->getAspectRatio();
	Ray r_primary = Ray(m_camera->getTransform().getPosition());

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			// Construct the ray's direction vector and aim it towards the virtual screen's pixel
			float x_norm = (x - w * 0.5f) / w * ar;
			float y_norm = (h * 0.5f - y) / h * ar;
			vec3 v_norm = vec3(x_norm, y_norm, -1.0f);

			// Rotate the ray direction based on camera look direction
			quaternion q = m_camera->getTransform().getRotation();
			quaternion q_inv = q.conjugate();
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

vec3 Engine::raytrace(Ray r, int n)
{
	// Immediately return vec3(0) if recursion limit was hit
	if (n > RECURSION_MAX)
		return vec3();

	// Initialize all raytracing data
	vec3 radiance = vec3();

	// Find the closest intersection
	Intersection xFinal = intersect(r);

	// Return black if no intersection happened
	if (xFinal == Intersection::invalidIntersection)
		return vec3();

	// Shade each hit surface agains each light source
	for (int i = 0; i < m_lights.size(); i++)
	{
		float NdotL, NdotH, L_length, A;
		Light L = m_lights[i];
		vec3 L_vector, V_vector, H_vector;

		if (L.getType() == LIGHT_DIRECTIONAL)
		{
			L_vector = L.getDirection();
			V_vector = r.getDirection().negate();
			H_vector = (V_vector + L_vector).normalize();
			A = 1.0f;
		}
		else if (L.getType() == LIGHT_POINT)
		{
			L_vector = L.getPosition() - xFinal.getPosition();
			L_length = L_vector.length();
			V_vector = r.getDirection().negate();
			H_vector = (V_vector + L_vector).normalize();
			A = L.getAttenuation().x + L.getAttenuation().y * L_length + L.getAttenuation().z * L_length * L_length + 1e-3f;
		}
		else
		{
			continue;
		}

		// Trace shadows
		Ray r_shadow = Ray(xFinal.getPosition(), L_vector);
		Intersection xShadow = intersect(r_shadow);

		if (xShadow != Intersection::invalidIntersection)
			continue;

		// Basic lambertian shading for now
		NdotL = std::max(vec3::dot(xFinal.getNormal(), L_vector.normalize()), 0.0f);
		NdotH = std::max(vec3::dot(xFinal.getNormal(), H_vector), 0.0f);
		radiance += (xFinal.getMaterial().getReflectance() * L.getColor() * NdotL * L.getIntensity()) / A;
		radiance += std::pow(NdotH, 50) / A;
	}

	return radiance;
}

Intersection Engine::intersect(Ray r)
{
	Intersection xInit = Intersection::invalidIntersection;
	Intersection xFinal = Intersection::invalidIntersection;
	float t_init = 1e6;

	// Find the nearest intersection against a sphere if there's any
	for (int i = 0; i < m_spheres.size(); i++)
	{
		xInit = m_spheres[i].intersect(r);

		if (xInit != Intersection::invalidIntersection && xInit.getT() < t_init)
		{
			xFinal = xInit;
			t_init = xFinal.getT();
		}
	}

	// Find the nearest intersection against a plane if there's any
	for (int i = 0; i < m_planes.size(); i++)
	{
		xInit = m_planes[i].intersect(r);

		if (xInit != Intersection::invalidIntersection && xInit.getT() < t_init)
		{
			xFinal = xInit;
			t_init = xFinal.getT();
		}
	}

	return xFinal;
}