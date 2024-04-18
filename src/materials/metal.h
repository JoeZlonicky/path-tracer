#pragma once

#include "../hittables/hit_record.h"
#include "../math/ray.h"
#include "../math/vector_3.h"
#include "material.h"

class Metal : public Material {
public:
	Metal(const Color& albedo, float fuzz);

	bool scatter(const Ray& r, const HitRecord& record, Color& attenuation_out, Ray& scattered_out) const override;

private:
	Color _albedo;
	float _fuzz;
};