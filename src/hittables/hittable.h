#pragma once

#include "../math/aabb.h"
#include "../math/interval.h"
#include "../math/ray.h"
#include "hit_record.h"

class Hittable {
public:
	virtual ~Hittable() = default;

	virtual bool hit(const Ray& r, const Interval& ray_t, HitRecord& record_out) const = 0;

	virtual AABB bounding_box() const = 0;
};