#pragma once

#include "material.h"

class Lambertian : public Material {
public:
	Lambertian(const Color& albedo) : _albedo(albedo) {}

private:
	Color _albedo;

	bool scatter(const Ray& r, const HitRecord& record, Color& attenuation_out, Ray& scattered_out) const override;
};