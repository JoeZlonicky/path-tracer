#pragma once

#include "vector_3.h"

class Ray {
public:
	Ray() {}

	Ray(const Point3& origin, const Vector3& direction) : _origin(origin), _direction(direction) {}

	Point3 getOrigin() const { return _origin; }
	Point3 getDirection() const { return _direction; }

	Point3 at(double t) const;

private:
	Point3 _origin;
	Vector3 _direction;
};