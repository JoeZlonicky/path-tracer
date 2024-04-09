#include "aabb.h"

AABB::AABB(const Interval& x, const Interval& y, const Interval& z) : x(x), y(y), z(z)
{
}

AABB::AABB(const Point3& a, const Point3& b)
{
	x = (a.x < b.x) ? Interval{ a.x, b.x } : Interval{ b.x, a.x };
	y = (a.y < b.y) ? Interval{ a.y, b.y } : Interval{ b.y, a.y };
	z = (a.z < b.z) ? Interval{ a.z, b.z } : Interval{ b.z, a.z };
}

bool AABB::hit(const Ray& r, Interval ray_t) const
{
	const Point3& ray_origin = r.getOrigin();
	const Vector3& ray_direction = r.getDirection();

	for (int axis = 0; axis < 3; ++axis)
	{
		const Interval& a = components[axis];
		auto inverse = 1.0 / ray_direction[axis];

		auto t0 = (components[axis].min - ray_origin[axis]) * inverse;
		auto t1 = (components[axis].max - ray_origin[axis]) * inverse;

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
