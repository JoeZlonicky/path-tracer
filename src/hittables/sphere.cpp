#include "sphere.h"

#include <cmath>
#include <memory>

#include "../materials/material.h"
#include "../math/aabb.h"
#include "../math/interval.h"
#include "../math/ray.h"
#include "../math/vector_3.h"
#include "hit_record.h"

Sphere::Sphere(Point3 center, float radius, std::shared_ptr<Material> material) : _center(center), _radius(radius), _material(material) {
	auto radius_v = Vector3{ radius, radius, radius };
	_bbox = { center - radius_v, center + radius_v };
}

bool Sphere::hit(const Ray& r, const Interval& ray_t, HitRecord& record_out) const {
	auto oc = r.getOrigin() - _center;
	auto dir = r.getDirection();
	auto r2 = _radius * _radius;

	// https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere/ray-sphereintersection
	auto a = dir.squared_magnitude();
	auto half_b = oc.dot(dir);
	auto c = oc.squared_magnitude() - r2;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0.f) return false;

	auto sqrt_d = sqrt(discriminant);

	// Find the nearest root
	auto root = (-half_b - sqrt_d) / a;
	if (!ray_t.surrounds(root)) {
		root = (-half_b + sqrt_d) / a;
		if (!ray_t.surrounds(root)) {
			return false;
		}
	}

	record_out.t = root;
	record_out.position = r.at(root);
	auto outward_normal = (record_out.position - _center) / _radius;
	record_out.set_face_normal(r, outward_normal);
	record_out.material = _material;

	return true;
}

AABB Sphere::bounding_box() const {
	return _bbox;
}
