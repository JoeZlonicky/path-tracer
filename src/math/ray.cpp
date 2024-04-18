#include "ray.h"

#include "vector_3.h"

Ray::Ray(const Point3& origin, const Vector3& direction) : _origin(origin), _direction(direction) {
}

Point3 Ray::getOrigin() const {
	return _origin;
}

Point3 Ray::getDirection() const {
	return _direction;
}

Point3 Ray::at(float t) const {
	return _origin + _direction * t;
}
