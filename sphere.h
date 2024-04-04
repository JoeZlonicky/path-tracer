#pragma once

#include "hittable.h"
#include "vector_3.h"

class Sphere : public Hittable {
public:
	Sphere(Point3 center, double radius);
	
	bool hit(const Ray& r, Interval ray_t, HitRecord& record) const override;

private:
	Point3 _center;
	double _radius;
};