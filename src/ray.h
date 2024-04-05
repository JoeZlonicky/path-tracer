#pragma once

#include "vector_3.h"

class Ray {
public:
	Ray() {}

	Ray(const Point3& origin, const Vector3& direction);

	Point3 getOrigin() const;
	Point3 getDirection() const;

	Point3 at(double t) const;

private:
	Point3 _origin;
	Vector3 _direction;
};