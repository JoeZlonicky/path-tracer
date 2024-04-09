#include "sphere.h"

#include <math.h>

Sphere::Sphere(Point3 center, double radius, std::shared_ptr<Material> material) : _center(center), _radius(radius), _material(material)
{
}

bool Sphere::hit(const Ray& r, Interval ray_t, HitRecord& record) const
{
	auto oc = r.getOrigin() - _center;
	auto dir = r.getDirection();
	auto r2 = _radius * _radius;

	// https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere/ray-sphereintersection
	auto a = dir.squared_magnitude();
	auto half_b = oc.dot(dir);
	auto c = oc.squared_magnitude() - r2;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0.0) return false;

	auto sqrt_d = sqrt(discriminant);

	// Find the nearest root
	auto root = (-half_b - sqrt_d) / a;
	if (!ray_t.surrounds(root)) {
		root = (-half_b + sqrt_d) / a;
		if (!ray_t.surrounds(root)) {
			return false;
		}
	}

	record.t = root;
	record.position = r.at(root);
	auto outward_normal = (record.position - _center) / _radius;
	record.set_face_normal(r, outward_normal);
	record.material = _material;

	return true;
}
