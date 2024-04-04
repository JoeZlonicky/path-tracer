#pragma once

#include "ray.h"

class HitRecord {
public:
	Point3 p;
	Vector3 normal;
	double t;
	bool is_front_face;

	void set_face_normal(const Ray& r, const Vector3& outward_normal) {
		// NOTE: `outward_normal` is assumed to have unit length
		is_front_face = r.getDirection().dot(outward_normal) < 0.0;
		normal = is_front_face ? outward_normal : -outward_normal;
	}
};

class Hittable {
public:
	virtual ~Hittable() = default;

	virtual bool hit(const Ray& r, double ray_t_min, double ray_t_max, HitRecord& record) const = 0;
};