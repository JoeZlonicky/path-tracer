#pragma once

#include <iosfwd>

struct Vector3 {
	union {
		struct {
			double x;
			double y;
			double z;
		};
		double components[3] = {0.f};
	};

	Vector3();
	explicit Vector3(double value);
	Vector3(double x, double y, double z);

	static Vector3 random();
	static Vector3 random(double min, double max);

	double& operator[](int i);
	const double& operator[](int i) const;

	Vector3& operator*=(double s);

	Vector3& operator/=(double s);

	Vector3& operator+=(const Vector3& v);

	Vector3& operator-=(const Vector3& v);

	void normalize();
	[[nodiscard]] Vector3 normalized() const;
	[[nodiscard]] bool is_near_zero() const;

	void clamp(double clamped_magnitude);
	[[nodiscard]] Vector3 clamped(double clamped_magnitude) const;

	[[nodiscard]] double squared_magnitude() const;
	[[nodiscard]] double magnitude() const;

	[[nodiscard]] Vector3 cross(const Vector3& v) const;
	[[nodiscard]] Vector3 reflect(const Vector3& n) const;
	[[nodiscard]] Vector3 refract(const Vector3& n, double etai_over_etat);

	[[nodiscard]] double dot(const Vector3& v) const;
};

using Point3 = Vector3;
using Color = Vector3;

Vector3 operator*(Vector3 a, const Vector3& b);
Vector3 operator*(Vector3 v, double s);
Vector3 operator*(double s, Vector3 v);

Vector3 operator/(Vector3 v, double s);

Vector3 operator+(Vector3 a, const Vector3& b);

Vector3 operator-(Vector3 a, const Vector3& b);
Vector3 operator-(Vector3 v);

std::ostream& operator<<(std::ostream& out, const Vector3& v);