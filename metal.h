#pragma once

#include "material.h"

class Metal : public Material {
public:
	Metal(const Color& albedo);

	bool scatter(const Ray& r, const HitRecord& record, Color& attenuation_out, Ray& scattered_out) const override;
	
private:
	Color _albedo;
};