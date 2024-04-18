#pragma once

#include <memory>

#include "../materials/material.h"
#include "../math/aabb.h"
#include "../math/interval.h"
#include "../math/ray.h"
#include "../math/vector_3.h"
#include "hit_record.h"
#include "hittable.h"

class Sphere : public Hittable {
public:
	Sphere(Point3 center, float radius, std::shared_ptr<Material> material);

	bool hit(const Ray& r, const Interval& ray_t, HitRecord& record_out) const override;

	[[nodiscard]] AABB bounding_box() const override;

private:
	Point3 _center;
	float _radius;
	AABB _bbox;
	std::shared_ptr<Material> _material;
};