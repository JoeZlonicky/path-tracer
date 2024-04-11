#pragma once

#include <memory>

#include "../materials/material.h"
#include "../math/aabb.h"
#include "../math/interval.h"
#include "../math/ray.h"
#include "../math/vector_3.h"
#include "hittable.h"

class Sphere : public Hittable {
public:
	Sphere(Point3 center, double radius, std::shared_ptr<Material> material);

	bool hit(const Ray& r, Interval ray_t, HitRecord& record) const override;

	AABB bounding_box() const override;

private:
	Point3 _center;
	double _radius;
	std::shared_ptr<Material> _material;
	AABB _bbox;
};