#include "metal.h"

#include "hittable.h"

Metal::Metal(const Color& albedo) : _albedo(albedo)
{
}

bool Metal::scatter(const Ray& r, const HitRecord& record, Color& attenuation_out, Ray& scattered_out) const
{
    Vector3 reflected = r.getDirection().normalized().reflect(record.normal);
    scattered_out = Ray{ record.p, reflected };
    attenuation_out = _albedo;
    return true;
}
