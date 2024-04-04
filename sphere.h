#pragma once

#include "hittable.h"
#include "vector_3.h"

class Sphere : public Hittable {
public:
	Sphere(Point3 center, double radius);
	
	bool hit(const Ray& r, double ray_t_min, double ray_t_max, HitRecord& record) const override;

private:
	Point3 _center;
	double _radius;
};