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

	bool hit(const Ray& r, Interval ray_t) const;
	int longest_axis() const;

	static AABB empty, universe;
};