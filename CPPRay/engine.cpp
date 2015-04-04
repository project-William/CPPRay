#include "engine.h"
#include "raytrace.h"
#include "pathtrace.h"

const vec3 Engine::NULLCOLOR = vec3();
const vec3 Engine::AMBIENTCOLOR = vec3(0.1f);

Engine::Engine(Display *display, Camera *camera)
{
	m_samples = new vec3[WIDTH * HEIGHT];
	clearSamples();
	m_display = display;
	m_camera = camera;



	//// SCENE DATA BELOW
	m_lights.push_back(Light(vec3(0, 2.5f, -8), vec3(1, 1, 1), vec3(0, 0, 0.2f), 1.0f)); // Raytracing light source
	Material mat_w_light = Material(vec3(5), vec3(0), 0, 0, 1, 0.1f, 1.0f, 0.9f); // Pathtracing light source

	Material mat_w_checkerboard = Material(vec3(1, 1, 1), 0, 1.0f, 1.0f, 0.9f, true);
	Material mat_w_diffuse = Material(vec3(), vec3(1), 0, 0, 1, 1.0f, 1.0f, 0.9f);
	Material mat_r_diffuse = Material(vec3(), vec3(1, 0, 0), 0, 0, 1, 1.0f, 1.0f, 0.9f);
	Material mat_g_diffuse = Material(vec3(), vec3(0, 1, 0), 0, 0, 1, 1.0f, 1.0f, 0.9f);
	Material mat_b_diffuse = Material(vec3(), vec3(0, 0, 1), 0, 0, 1, 1.0f, 1.0f, 0.9f);
	Material mat_r_reflective = Material(vec3(), vec3(1, 0, 0), 1.0f, 0, 1, 0.1f, 0.66f, 0.5f);
	Material mat_g_reflective = Material(vec3(), vec3(0, 1, 0), 1.0f, 0, 1, 0.1f, 0.75f, 0.75f);
	Material mat_b_reflective = Material(vec3(), vec3(0, 0, 1), 1.0f, 0, 1, 0.25f, 0.75f, 0.5f);
	Material mat_c_reflective = Material(vec3(), vec3(0, 0, 1), 1.0f, 0, 1, 0.15f, 0.9f, 0.1f);
	Material mat_mirror = Material(vec3(), vec3(1), 1, 0, 1, 0.05f, 0.75f, 0.1f);
	Material mat_water = Material(vec3(), vec3(0), 0, 1, 1.33f, 0.1f, 1.0f, 0.9f);
	Material mat_glass = Material(vec3(), vec3(0), 0, 1, 1.52f, 0.1f, 1.0f, 0.9f);

	m_spheres.push_back(Sphere(vec3(2.5f, 1, -6), 1, mat_r_reflective));
	m_spheres.push_back(Sphere(vec3(-2.5f, 1, -6), 1, mat_b_reflective));
	m_spheres.push_back(Sphere(vec3(1, 0.75f, -4), 0.5f, mat_g_reflective));
	m_spheres.push_back(Sphere(vec3(-0.5f, 1, -8), 1, mat_c_reflective));
	m_spheres.push_back(Sphere(vec3(0.5f, 2.5f, -5), 1, mat_w_light));

	m_planes.push_back(Plane(vec3(0, 0, 0), vec3(0, 1, 0), mat_w_diffuse));
	m_planes.push_back(Plane(vec3(0, 5, 0), vec3(0, -1, 0), mat_w_diffuse));
	m_planes.push_back(Plane(vec3(5, 0, 0), vec3(-1, 0, 0), mat_w_diffuse));
	m_planes.push_back(Plane(vec3(-5, 0, 0), vec3(1, 0, 0), mat_w_diffuse));
	m_planes.push_back(Plane(vec3(0, 0, -16), vec3(0, 0, 1), mat_w_diffuse));
	m_planes.push_back(Plane(vec3(0, 0, 4), vec3(0, 0, -1), mat_w_diffuse));
}

Engine::~Engine()
{
	delete[] m_samples;

	std::cout << "Engine object has been destroyed succesfully." << std::endl;
}

void Engine::update(float dt)
{

}

void Engine::renderRT()
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

void Engine::renderPT()
{
	auto w = m_display->getWidth();
	auto h = m_display->getHeight();
	auto ar = m_display->getAspectRatio();
	auto r_primary = Ray(m_camera->getTransform().getPosition());
	auto &q = m_camera->getTransform().getRotation();
	auto &q_inv = q.conjugate();

	// Increase samplecounter by one
	m_sampleAmount++;

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			// Pixel's color value
			auto &radiance = vec3();

			// Construct the ray's direction vector and aim it towards the virtual screen's pixel
			auto x_norm = (x - w * 0.5f) / w * ar;
			auto y_norm = (h * 0.5f - y) / h;
#if SUPERSAMPLINGLEVEL>1
			for (unsigned int i = 0; i < SUPERSAMPLINGLEVEL; i++)
			{
				// Random jitter per ray for supersampling
				auto x_rng = 2 * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) - 1;
				auto y_rng = 2 * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) - 1;

				// Apply the jitter to the ray dir
				auto v_norm = vec3(x_norm + 1e-3f * x_rng, y_norm + 1e-3f * y_rng, -1.0f);

				// Rotate the ray direction based on camera look direction
				auto w = quaternion(0, v_norm.x, v_norm.y, v_norm.z);
				auto r = q * w * q_inv;

				// Set the final direction of the rotated ray dir
				r_primary.setDirection(vec3(r.x, r.y, r.z));

				// Pathtrace
				radiance += pathtrace(r_primary, 0, 1);
			}
#else
			// Create the ray's direction vector based on normalized screenspace coordinates
			auto v_norm = vec3(x_norm, y_norm, -1.0f);

			// Rotate the ray direction based on camera look direction
			auto w = quaternion(0, v_norm.x, v_norm.y, v_norm.z);
			auto r = q * w * q_inv;

			// Set the final direction of the rotated ray dir
			r_primary.setDirection(vec3(r.x, r.y, r.z));

			// Pathtrace
			radiance = pathtrace(r_primary, 0, 1);
#endif

			// Add the sample to the samples array
			m_samples[x + y * WIDTH] += radiance / SUPERSAMPLINGLEVEL;

			// Set the pixel color to the desired value
			m_display->setPixel(x, y, m_samples[x + y * WIDTH] / static_cast<float>(m_sampleAmount));
		}
	}
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

void Engine::clearSamples()
{
	for (unsigned int i = 0; i < WIDTH * HEIGHT; i++)
	{
		m_samples[i] = vec3(0);
	}

	m_sampleAmount = 0;
}