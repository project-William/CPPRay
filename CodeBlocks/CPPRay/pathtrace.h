vec3 Engine::pathtrace(const Ray &r, int n, float weight, unsigned short *Xi)
{
	// Immediately return vec3(0) if recursion limit was hit
	if (n > RECURSION_MAX || weight <= 0.0f)
		return COLOR_NULL;

	// Initialize all pathtracing data
	auto radiance = vec3();

	// Find the closest intersection
	Intersection xFinal = intersect(r, MAXDISTANCE);

	// Return black if no intersection happened
	if (xFinal == Intersection::invalidIntersection)
		return COLOR_NULL;

	// Return the emittance immediately if we hit an emissive surface
	if (xFinal.getMaterial().getEmittance().length() > 0.0f)
		return xFinal.getMaterial().getEmittance() * weight;

	// Take intersection info & ray info and assign it to references with short names
	auto V_vector = r.getDirection();
	auto P_vector = xFinal.getPosition();
	auto N_vector = xFinal.getNormal();
	auto Nr_vector = vec3::dot(N_vector, V_vector) < 0.0f ? N_vector : N_vector.negate();
	auto M_info = xFinal.getMaterial();
	auto f = M_info.getReflectance();

	// Russian roulette, use maximum reflectance amount
	float p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z;
	if (n > RECURSION_MAX/2 || !p)
		if (math::pseudorand(Xi) < p)
			f = f * (1.0f / p);
		else
			return M_info.getEmittance();

	// Diffuse surfaces
	if (M_info.getReflT() == DIFF)
	{
		// Calculate the light direction ray
		auto L_rand = vec3::sampleHemisphere(Nr_vector, Xi);

		// Prepare all required information
		auto L_vector = L_rand.negate();
		auto H_vector = (V_vector + L_vector).normalize();

		// Calculate all dot products
		auto NdotL = std::abs(vec3::dot(Nr_vector, L_vector));
		auto NdotV = std::abs(vec3::dot(Nr_vector, V_vector));
		auto NdotH = std::abs(vec3::dot(Nr_vector, H_vector));
		auto VdotH = std::abs(vec3::dot(V_vector, H_vector));

		// Calculate the bidirectional reflectance function value
		vec3 BRDF = M_info.getReflectance() * NdotL;

		// Calculate the contributed reflected light amount
		vec3 REFL = pathtrace(Ray(P_vector, L_rand), n + 1, weight, Xi);

		// Apply the final color to the radiance amount
		return BRDF * REFL * PI_1;
	}
	// Mirror surfaces
	else if (M_info.getReflT() == SPEC)
	{
		return f * pathtrace(Ray(P_vector, vec3::reflect(V_vector, N_vector)), n + 1, weight, Xi);
	}
	// Dielectric surfaces
	else if (M_info.getReflT() == REFR)
	{
		Ray r_reflected = Ray(P_vector, vec3::reflect(V_vector, N_vector));
		bool into = vec3::dot(N_vector, Nr_vector) > 0.0f;
		float nc = 1, nt = M_info.getIOR(), nnt = into ? nc / nt : nt / nc, ddn = vec3::dot(V_vector, Nr_vector), cos2t;

		// If total internal reflection, reflect
		if ((cos2t = 1.0f - nnt * nnt * (1.0f - ddn * ddn)) < 0.0f)
			return f * pathtrace(r_reflected, n + 1, weight, Xi);

		// Otherwise, choose reflection or refraction
		vec3 tdir = (V_vector * nnt - N_vector * ((into ? 1.0f : -1.0f) * (ddn * nnt + std::sqrt(cos2t)))).normalize();
		float a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1.0f - (into ? -ddn : vec3::dot(tdir, N_vector));
		float Re = R0 + (1.0f - R0) * c * c * c * c * c, Tr = 1.0f - Re, P = 0.25f + 0.5f * Re, RP = Re / P, TP = Tr / (1.0f - P);
		return f * (n > 2 ? (math::pseudorand(Xi) < P ?
			pathtrace(r_reflected, n + 1, weight, Xi) * RP : pathtrace(Ray(P_vector, tdir), n + 1, weight, Xi) * TP) :
			pathtrace(r_reflected, n + 1, weight, Xi) * Re + pathtrace(Ray(P_vector, tdir), n + 1, weight, Xi) * Tr);
	}

	return radiance;
}
