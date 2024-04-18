#pragma once

#include "vector_3.h"

class Ray {
public:
	Ray() = default;
	Ray(const Point3& origin, const Vector3& direction);

	[[nodiscard]] Point3 getOrigin() const;
	[[nodiscard]] Point3 getDirection() const;

	[[nodiscard]] Point3 at(double t) const;

private:
	Point3 _origin;
	Vector3 _direction;  // Not necessarily normalized
};