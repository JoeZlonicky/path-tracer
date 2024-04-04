#pragma once

#include "material.h"

class Dielectric : public Material {
public:
	Dielectric(double refraction_index);

	bool scatter(const Ray& r, const HitRecord& record, Color& attenuation_out, Ray& scattered_out) const override;

private:
	double _refraction_index;
};