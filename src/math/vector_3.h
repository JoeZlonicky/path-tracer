#pragma once

#include <iosfwd>

struct Vector3 {
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float components[3] = {0.f};
	};

	Vector3();
	explicit Vector3(float value);
	Vector3(float x, float y, float z);

	static Vector3 random();
	static Vector3 random(float min, float max);

	float& operator[](int i);
	const float& operator[](int i) const;

	Vector3& operator*=(float s);

	Vector3& operator/=(float s);

	Vector3& operator+=(const Vector3& v);

	Vector3& operator-=(const Vector3& v);

	void normalize();
	[[nodiscard]] Vector3 normalized() const;
	[[nodiscard]] bool is_near_zero() const;

	void clamp(float clamped_magnitude);
	[[nodiscard]] Vector3 clamped(float clamped_magnitude) const;

	[[nodiscard]] float squared_magnitude() const;
	[[nodiscard]] float magnitude() const;

	[[nodiscard]] Vector3 cross(const Vector3& v) const;
	[[nodiscard]] Vector3 reflect(const Vector3& n) const;
	[[nodiscard]] Vector3 refract(const Vector3& n, float etai_over_etat);

	[[nodiscard]] float dot(const Vector3& v) const;
};

using Point3 = Vector3;
using Color = Vector3;

Vector3 operator*(Vector3 a, const Vector3& b);
Vector3 operator*(Vector3 v, float s);
Vector3 operator*(float s, Vector3 v);

Vector3 operator/(Vector3 v, float s);

Vector3 operator+(Vector3 a, const Vector3& b);

Vector3 operator-(Vector3 a, const Vector3& b);
Vector3 operator-(Vector3 v);

std::ostream& operator<<(std::ostream& out, const Vector3& v);