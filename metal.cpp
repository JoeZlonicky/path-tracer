#include "metal.h"

#include "hittable.h"
#include "utility.h"

Metal::Metal(const Color& albedo, double fuzz) : _albedo(albedo), _fuzz(fuzz <= 1.0 ? fuzz : 1.0)
{
}

bool Metal::scatter(const Ray& r, const HitRecord& record, Color& attenuation_out, Ray& scattered_out) const
{
    Vector3 reflected = r.getDirection().normalized().reflect(record.normal);
    scattered_out = Ray{ record.p, reflected + _fuzz * random_unit_vector()};
    attenuation_out = _albedo;

    auto did_fuzz_place_below_surface = scattered_out.getDirection().dot(record.normal) <= 0;
    return !did_fuzz_place_below_surface;
}
