#include "dielectric.h"

#include "hittable.h"

Dielectric::Dielectric(double refraction_index) : _refraction_index(refraction_index)
{
}

bool Dielectric::scatter(const Ray& r, const HitRecord& record, Color& attenuation_out, Ray& scattered_out) const
{
    attenuation_out = Color{ 1.0, 1.0, 1.0 };
    double refraction_ratio = record.is_front_face ? (1.0 / _refraction_index) : _refraction_index;

    auto unit_direction = r.getDirection().normalized();
    auto refracted = unit_direction.refract(record.normal, refraction_ratio);

    scattered_out = { record.p, refracted };
    return true;
}
