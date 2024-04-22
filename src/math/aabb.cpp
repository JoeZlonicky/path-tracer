#include "aabb.h"

#include "interval.h"
#include "ray.h"
#include "vector_3.h"

AABB AABB::empty{ Interval::empty, Interval::empty, Interval::empty };
AABB AABB::universe{ Interval::universe, Interval::universe, Interval::universe };

AABB::AABB(const Interval& x, const Interval& y, const Interval& z) : x(x), y(y), z(z) {
}

AABB::AABB(const Point3& a, const Point3& b) {
	x = (a.x < b.x) ? Interval{ a.x, b.x } : Interval{ b.x, a.x };
	y = (a.y < b.y) ? Interval{ a.y, b.y } : Interval{ b.y, a.y };
	z = (a.z < b.z) ? Interval{ a.z, b.z } : Interval{ b.z, a.z };
}

AABB::AABB(const AABB& a, const AABB& b) {
	x = { a.x, b.x };
	y = { a.y, b.y };
	z = { a.z, b.z };
}

bool AABB::intersect(const Ray& r, Interval ray_t) const {
	const Point3& ray_origin = r.getOrigin();
	const Vector3& ray_direction = r.getDirection();

	for (int axis = 0; axis < 3; ++axis) {
		const Interval& a = intervals[axis];
		auto inverse = 1.f / ray_direction[axis];

		auto t0 = (a.min - ray_origin[axis]) * inverse;
		auto t1 = (a.max - ray_origin[axis]) * inverse;

		if (t0 < t1) {
			if (t0 > ray_t.min) ray_t.min = t0;
			if (t1 < ray_t.max) ray_t.max = t1;
		}
		else {
			if (t1 > ray_t.min) ray_t.min = t1;
			if (t0 < ray_t.max) ray_t.max = t0;
		}

		if (ray_t.max <= ray_t.min) return false;
	}

	return true;
}

int AABB::longest_axis() const {
	if (x.size() > y.size()) {
		return x.size() > z.size() ? 0 : 2;
	}
	return y.size() > z.size() ? 1 : 2;
}

void AABB::pad_to_min_size(float s) {
	if (x.size() < s) x.expand(s);
	if (y.size() < s) y.expand(s);
	if (z.size() < s) z.expand(s);
}
