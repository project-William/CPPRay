#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
	Scene()
	{
		//// SCENE DATA BELOW
		Material mat_l_w = Material(vec3(50, 50, 50), vec3(0), 1, DIFF);
		Material mat_l_r = Material(vec3(16, 4, 4), vec3(0), 1, DIFF);
		Material mat_l_g = Material(vec3(4, 16, 4), vec3(0), 1, DIFF);
		Material mat_l_b = Material(vec3(4, 4, 16), vec3(0), 1, DIFF);
		Material mat_d_w = Material(vec3(), vec3(0.75f, 0.75f, 0.75f), 1, DIFF);
		Material mat_d_r = Material(vec3(), vec3(0.75f, 0.25f, 0.25f), 1, DIFF);
		Material mat_d_g = Material(vec3(), vec3(0.25f, 0.75f, 0.25f), 1, DIFF);
		Material mat_d_b = Material(vec3(), vec3(0.25f, 0.25f, 0.75f), 1, DIFF);
		Material mat_d_y = Material(vec3(), vec3(0.75f, 0.75f, 0.25f), 1, DIFF);
		Material mat_m_w = Material(vec3(), vec3(1, 1, 1), 1, SPEC);
		Material mat_m_r = Material(vec3(), vec3(1, 0.25f, 0.25f), 1, SPEC);
		Material mat_m_g = Material(vec3(), vec3(0.25f, 1, 0.25f), 1, SPEC);
		Material mat_m_b = Material(vec3(), vec3(0.25f, 0.25f, 0.25f), 1, SPEC);
		Material mat_m_y = Material(vec3(), vec3(0.75f, 1, 0.25f), 1, SPEC);
		Material mat_g_w = Material(vec3(), vec3(1, 1, 1), 1.52f, REFR);
		Material mat_g_c = Material(vec3(), vec3(0.75f, 1, 1), 1.52f, REFR);
		Material mat_g_y = Material(vec3(), vec3(1, 0.75f, 0.25f), 1.52f, REFR);
		Material mat_g_p = Material(vec3(), vec3(0.75f, 0.25f, 1), 1.52f, REFR);

		m_spheres.push_back(Sphere(vec3(0, 4.75f, -5), 1, mat_l_w)); // Light
		m_spheres.push_back(Sphere(vec3(1, 0.5f, -5), 0.5f, mat_m_w));
		m_spheres.push_back(Sphere(vec3(-1, 0.5f, -4), 0.5f, mat_g_w));
		m_spheres.push_back(Sphere(vec3(1, 0.25f, -4), 0.25f, mat_g_y));
		m_spheres.push_back(Sphere(vec3(0.25f, 0.25f, -3.75f), 0.25f, mat_g_p));
		m_spheres.push_back(Sphere(vec3(-1.5f, 2.5f, -6), 0.5f, mat_g_c));

		m_planes.push_back(Plane(vec3(0, 0, 0), vec3(0, 1, 0), mat_d_w));
		m_planes.push_back(Plane(vec3(0, 4, 0), vec3(0, -1, 0), mat_d_w));
		m_planes.push_back(Plane(vec3(2, 0, 0), vec3(-1, 0, 0), mat_d_b));
		m_planes.push_back(Plane(vec3(-2, 0, 0), vec3(1, 0, 0), mat_d_r));
		m_planes.push_back(Plane(vec3(0, 0, -8), vec3(0, 0, 1), mat_d_w));
		m_planes.push_back(Plane(vec3(0, 0, 0), vec3(0, 0, -1), mat_d_w));
	}

	std::vector<Sphere> getSceneSpheres()
	{
		return m_spheres;
	}

	std::vector<Plane> getScenePlanes()
	{
		return m_planes;
	}
private:
	std::vector<Sphere> m_spheres;
	std::vector<Plane> m_planes;
protected:
};

#endif