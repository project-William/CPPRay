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