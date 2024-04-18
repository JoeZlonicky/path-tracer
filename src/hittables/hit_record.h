#pragma once

#include <memory>

#include "../math/ray.h"
#include "../math/vector_3.h"

class Material;

struct HitRecord {
	Point3 position;
	Vector3 normal;
	std::shared_ptr<Material> material;
	float t = 0.f;
	bool is_front_face = false;

	// NOTE: `outward_normal` is assumed to have unit length
	void set_face_normal(const Ray& r, const Vector3& outward_normal);
};