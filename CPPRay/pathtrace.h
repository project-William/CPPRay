vec3 Engine::pathtrace(const Ray &r, int n, float weight)
{
	// Immediately return vec3(0) if recursion limit was hit
	if (n > RECURSION_MAX || weight <= 0.0f)
		return NULLCOLOR;

	// Initialize all pathtracing data
	auto radiance = vec3();

	// Find the closest intersection
	Intersection xFinal = intersect(r, MAXDISTANCE);

	// Return black if no intersection happened
	if (xFinal == Intersection::invalidIntersection)
		return NULLCOLOR;

	// Return the emittance immediately if we hit an emissive surface
	if (xFinal.getMaterial().getEmittance().length() > 0.0f)
		return xFinal.getMaterial().getEmittance() * weight;

	// Initialize some variables that are not going to change
	auto &V_vector = r.getDirection();
	auto &P_vector = xFinal.getPosition();
	auto &N_vector = xFinal.getNormal();
	auto &M_info = xFinal.getMaterial();

	// Cook-torrance diffuse BRDF
	if (M_info.getReflectivity() <= 0.0f && M_info.getReflectance().length() > 0.0f)
	{
		// Get surface properties
		auto K = M_info.getDensity();

		// Calculate the light direction ray
		auto L_rand = vec3::randomHemisphere(N_vector);

		// Prepare all required information
		auto &L_vector = L_rand.negate();
		auto &H_vector = (V_vector + L_vector).normalize();

		// Calculate all dot products
		auto NdotL = std::abs(vec3::dot(N_vector, L_vector));
		auto NdotV = std::abs(vec3::dot(N_vector, V_vector));
		auto NdotH = std::abs(vec3::dot(N_vector, H_vector));
		auto VdotH = std::abs(vec3::dot(V_vector, H_vector));

		// Evaluate the geometric term
		auto geo_numer = 2.0f * NdotH;
		auto geo_denom = VdotH;
		auto geometric = std::min(1.0f, std::min((geo_numer * NdotV) / geo_denom,(geo_numer * NdotL) / geo_denom));

		// Calculate the final shading value
		float Rs = geometric / (NdotV * NdotL * PI + 1e-3f);

		// Calculate the bidirectional reflectance function value
		vec3 BRDF = M_info.getReflectance() * K + (Rs * (1.0f - K));

		// Calculate the contributed final light amount
		vec3 REFL = pathtrace(Ray(P_vector, L_rand), n + 1, weight) * NdotL;

		// Apply the final color to the radiance amount
		radiance += BRDF * REFL;
	}

	// Cook-torrance specular BRDF
	if (M_info.getReflectivity() > 0.0f)
	{
		// Modify ray weight
		weight *= M_info.getReflectivity() * M_info.getFresnel();

		// Get surface properties
		auto R = M_info.getRoughness();
		auto F = M_info.getFresnel();
		auto K = M_info.getDensity();

		// Calculate the light direction ray
		auto L_rand = vec3::randomHemisphere(N_vector);
		auto L_mirr = vec3::reflect(V_vector, N_vector);
		vec3 w = L_mirr;
		vec3 u = vec3::cross(N_vector, w);
		vec3 v = vec3::cross(u, v);
		vec3 L = (u * L_rand.x + v * L_rand.y + w * L_rand.z).normalize();
		if (vec3::dot(N_vector, L) < 0.0f)
			L = (u * L_rand.x - v * L_rand.y + w * L_rand.z).normalize();

		// Prepare all required information
		auto &L_vector = L.negate();
		auto &H_vector = (V_vector + L_vector).normalize();

		// Calculate all dot products
		auto NdotL = std::abs(vec3::dot(N_vector, L_vector));
		auto NdotV = std::abs(vec3::dot(N_vector, V_vector));
		auto NdotH = std::abs(vec3::dot(N_vector, H_vector));
		auto VdotH = std::abs(vec3::dot(V_vector, H_vector));

		// Evaluate the geometric term
		auto geo_numer = 2.0f * NdotH;
		auto geo_denom = VdotH;
		auto geometric = std::min(1.0f, std::min((geo_numer * NdotV) / geo_denom,(geo_numer * NdotL) / geo_denom));

		// Evaluate the roughness term
		float rough_a = 1.0f / (4.0f * R * R * std::pow(NdotH, 4));
		float rough_b = NdotH * NdotH - 1.0f;
		float rough_c = R * R * NdotH * NdotH;
		float roughness = rough_a * std::exp(rough_b / rough_c);

		// Evaluate the fresnel term
		float fresnel = std::pow(1.0f - VdotH, 5);
		fresnel *= 1.0f - F;
		fresnel += F;

		// Calculate the final shading value
		float Rs = geometric * fresnel * roughness / (NdotV * NdotL * PI + 1e-3f);

		// Calculate the bidirectional reflectance function value
		vec3 BRDF = M_info.getReflectance() * K + (Rs * (1.0f - K));

		// Calculate the contributed final light amount
		vec3 REFL = pathtrace(Ray(P_vector, L), n + 1, weight) * NdotL;

		// Apply the final color to the radiance amount
		radiance += BRDF * REFL;
	}

	return radiance;
}