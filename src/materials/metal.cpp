#include "metal.h"

#include "../hittables/hit_record.h"
#include "../math/math_utility.h"
#include "../math/ray.h"
#include "../math/vector_3.h"

Metal::Metal(const Color& albedo, float fuzz) : _albedo(albedo), _fuzz(fuzz <= 1.f ? fuzz : 1.f) {
}

bool Metal::scatter(const Ray& r, const HitRecord& record, Color& attenuation_out, Ray& scattered_out) const {
	auto reflected = r.getDirection().normalized().reflect(record.normal);
	scattered_out = {record.position, reflected + _fuzz * MathUtility::random_unit_vector()};
	attenuation_out = _albedo;

	auto did_fuzz_place_below_surface = scattered_out.getDirection().dot(record.normal) <= 0;
	return !did_fuzz_place_below_surface;
}
