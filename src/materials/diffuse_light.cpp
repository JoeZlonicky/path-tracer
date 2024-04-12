#include "diffuse_light.h"

#include "../math/vector_3.h"

DiffuseLight::DiffuseLight(Color color) : _color(color) {
}

Color DiffuseLight::emitted() const {
	return _color;
}

bool DiffuseLight::scatter(const Ray& r, const HitRecord& record, Color& attenuation_out, Ray& scattered_out) const {
	return false;
}
