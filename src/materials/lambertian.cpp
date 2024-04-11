#include "lambertian.h"

#include "../hittables/hit_record.h"
#include "../math/math_utility.h"
#include "../math/ray.h"
#include "../math/vector_3.h"

bool Lambertian::scatter(const Ray& r, const HitRecord& record, Color& attenuation_out, Ray& scattered_out) const {
	auto scatter_direction = record.normal + MathUtility::random_unit_vector();
	scattered_out = Ray{record.position, scatter_direction};

	// If random unit vector perfectly cancels out normal
	if(scatter_direction.is_near_zero()) {
		scatter_direction = record.normal;
	}

	attenuation_out = _albedo;
	return true;
}
