#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <random>

#include "../math/interval.h"
#include "../math/vector_3.h"

namespace MathUtility {
	constexpr double infinity = std::numeric_limits<double>::infinity();
	constexpr double pi = 3.1415926535897932385;

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

	inline int random_range(int min, int max) {
		return int(random_range(double(min), double(max)));
	}

	inline Vector3 random_in_unit_sphere() {
		while(true) {
			auto p = Vector3::random(-1.0, 1.0);
			if(p.squared_magnitude() < 1) return p;
		}
	}

	inline Vector3 random_unit_vector() {
		return random_in_unit_sphere().normalized();
	}

	inline Vector3 random_on_hemisphere(const Vector3& normal) {
		Vector3 on_unit_sphere = random_unit_vector();

		if(on_unit_sphere.dot(normal) > 0.0) return on_unit_sphere;
		return -on_unit_sphere;
	}

	inline Vector3 random_in_unit_disk() {
		while(true) {
			auto p = Vector3{random_range(-1.0,1.0), random_range(-1.0, 1.0), 0.0};
			if(p.squared_magnitude() < 1.0) return p;
		}
	}

	inline double linear_to_gamma(double linear) {
		return sqrt(linear);
	}

	inline void write_color_256(std::ostream& out, Color pixel_color) {
		auto r = pixel_color.x;
		auto g = pixel_color.y;
		auto b = pixel_color.z;

		r = linear_to_gamma(r);
		g = linear_to_gamma(g);
		b = linear_to_gamma(b);

		Interval intensity{0.000, 0.999};
		constexpr int max_value = 256;

		out << static_cast<int>(max_value * intensity.clamped(r)) << ' '
			<< static_cast<int>(max_value * intensity.clamped(g)) << ' '
			<< static_cast<int>(max_value * intensity.clamped(b)) << '\n';
	}
}
