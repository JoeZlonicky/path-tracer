#include "dielectric.h"

#include <cmath>

#include "../hittables/hit_record.h"
#include "../math/math_utility.h"
#include "../math/ray.h"
#include "../math/vector_3.h"

namespace {
	double reflectance(double cosine, double refraction_ratio) {
		// Schlick's approximation
		auto r0 = (1.0 - refraction_ratio) / (1.0 + refraction_ratio);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
}

Dielectric::Dielectric(double refraction_index) : _refraction_index(refraction_index) {
}

bool Dielectric::scatter(const Ray& r, const HitRecord& record, Color& attenuation_out, Ray& scattered_out) const {
	attenuation_out = Color{1.0, 1.0, 1.0};
	double refraction_ratio = record.is_front_face ? (1.0 / _refraction_index) : _refraction_index;

	auto unit_direction = r.getDirection().normalized();
	auto cos_theta = fmin(unit_direction.dot(-record.normal), 1.0);
	auto sin_theta = sqrt(1.0 - cos_theta * cos_theta);

	auto can_refract = refraction_ratio * sin_theta <= 1.0;
	Vector3 direction;

	if(can_refract && reflectance(cos_theta, refraction_ratio) <= MathUtility::random_normalized()) {
		direction = unit_direction.refract(record.normal, refraction_ratio);
	} else {
		direction = unit_direction.reflect(record.normal);
	}

	scattered_out = {record.position, direction};
	return true;
}
