#pragma once

#include "../math/vector_3.h"
#include "material.h"

class DiffuseLight : public Material {
public:
	DiffuseLight(Color color);

	bool scatter(const Ray& r, const HitRecord& record, Color& attenuation_out, Ray& scattered_out) const override;
	Color emitted() const override;
private:
	Color _color;
};