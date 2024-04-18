#pragma once

#include "interval.h"
#include "ray.h"
#include "vector_3.h"

class AABB {
public:
	union {
		struct {
			Interval x, y, z;
		};
		Interval intervals[3]{};
	};


	AABB() = default;
	AABB(const Interval& x, const Interval& y, const Interval& z);
	AABB(const Point3& a, const Point3& b);
	AABB(const AABB& a, const AABB& b);

	[[nodiscard]] bool intersect(const Ray& r, Interval ray_t) const;
	[[nodiscard]] int longest_axis() const;
	void pad_to_min_size(float s);

	static AABB empty, universe;
};