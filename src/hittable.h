#pragma once

#include "aabb.h"
#include "interval.h"
#include "material.h"
#include "ray.h"

class HitRecord {
public:
	Point3 position;
	Vector3 normal;
	std::shared_ptr<Material> material;
	double t;
	bool is_front_face = false;

	void set_face_normal(const Ray& r, const Vector3& outward_normal) {
		// NOTE: `outward_normal` is assumed to have unit length
		is_front_face = r.getDirection().dot(outward_normal) < 0.0;
		normal = is_front_face ? outward_normal : -outward_normal;
	}
};

class Hittable {
public:
	virtual ~Hittable() = default;

	virtual bool hit(const Ray& r, Interval ray_t, HitRecord& record) const = 0;

	virtual AABB bounding_box() const = 0;
};