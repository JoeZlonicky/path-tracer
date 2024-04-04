#pragma once

#include <cmath>
#include <random>
#include <limits>

#include "vector_3.h"

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

inline double random_normalized() {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_range(double min, double max) {
	return min + (max - min) * random_normalized();
}

inline Vector3 random_in_unit_sphere() {
	while (true) {
		auto p = Vector3::random(-1.0, 1.0);
		if (p.squared_magnitude() < 1) return p;
	}
}

inline Vector3 random_unit_vector() {
	return random_in_unit_sphere().normalized();
}

inline Vector3 random_on_hemisphere(const Vector3& normal) {
	Vector3 on_unit_sphere = random_unit_vector();

	if (on_unit_sphere.dot(normal) > 0.0) return on_unit_sphere;
	return -on_unit_sphere;
}