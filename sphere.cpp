#include "sphere.h"

#include <math.h>

Sphere::Sphere(Point3 center, double radius) : _center(center), _radius(radius)
{
}

bool Sphere::hit(const Ray& r, double ray_t_min, double ray_t_max, HitRecord& record) const
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
	if (root < ray_t_min || root > ray_t_max) {
		root = (-half_b + sqrt_d) / a;
		if (root < ray_t_min || root > ray_t_max) {
			return false;
		}
	}

	record.t = root;
	record.p = r.at(root);
	Vector3 outward_normal = (record.p - _center) / _radius;
	record.set_face_normal(r, outward_normal);

	return true;
}
