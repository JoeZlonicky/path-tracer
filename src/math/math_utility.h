#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <random>

#include "../math/interval.h"
#include "../math/vector_3.h"

namespace MathUtility {
	constexpr float infinity = std::numeric_limits<float>::infinity();
	constexpr float pi = 3.1415926535897932385f;

	inline float degrees_to_radians(float degrees) {
		return degrees * pi / 180.f;
	}

	inline float random_normalized() {
		static std::uniform_real_distribution<float> distribution(0.f, 1.f);
		static std::mt19937 generator;
		return distribution(generator);
	}

	inline float random_range(float min, float max) {
		return min + (max - min) * random_normalized();
	}

	inline int random_range(int min, int max) {
		return int(random_range(static_cast<float>(min), static_cast<float>(max)));
	}

	inline Vector3 random_in_unit_sphere() {
		while(true) {
			auto p = Vector3::random(-1.f, 1.f);
			if(p.squared_magnitude() < 1) return p;
		}
	}

	inline Vector3 random_unit_vector() {
		return random_in_unit_sphere().normalized();
	}

	inline Vector3 random_on_hemisphere(const Vector3& normal) {
		Vector3 on_unit_sphere = random_unit_vector();

		if(on_unit_sphere.dot(normal) > 0.f) return on_unit_sphere;
		return -on_unit_sphere;
	}

	inline Vector3 random_in_unit_disk() {
		while(true) {
			auto p = Vector3{random_range(-1.f,1.f), random_range(-1.f, 1.f), 0.f};
			if(p.squared_magnitude() < 1.f) return p;
		}
	}

	inline float linear_to_gamma(float linear) {
		return sqrt(linear);
	}

	inline void write_color_256(std::ostream& out, Color pixel_color) {
		auto r = pixel_color.x;
		auto g = pixel_color.y;
		auto b = pixel_color.z;

		r = linear_to_gamma(r);
		g = linear_to_gamma(g);
		b = linear_to_gamma(b);

		Interval intensity{0.f, 0.999f};
		constexpr int max_value = 256;

		out << static_cast<int>(max_value * intensity.clamped(r)) << ' '
			<< static_cast<int>(max_value * intensity.clamped(g)) << ' '
			<< static_cast<int>(max_value * intensity.clamped(b)) << '\n';
	}
}
