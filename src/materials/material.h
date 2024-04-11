#pragma once

#include "../math/ray.h"
#include "../math/vector_3.h"

class HitRecord;

class Material {
public:
	virtual ~Material() = default;

	virtual bool scatter(const Ray& r, const HitRecord& record, Color& attenuation_out, Ray& scattered_out) const = 0;
};