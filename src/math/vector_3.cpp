#include "vector_3.h"

#include <cmath>
#include <fstream>

#include "math_utility.h"

Vector3::Vector3() : x(0.f), y(0.f), z(0.f) {}

Vector3::Vector3(float value) : x(value), y(value), z(value) {}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

Vector3 Vector3::random() {
	return {MathUtility::random_normalized(), MathUtility::random_normalized(), MathUtility::random_normalized()};
}

Vector3 Vector3::random(float min, float max) {
	return {MathUtility::random_range(min, max), MathUtility::random_range(min, max), MathUtility::random_range(min, max)};
}

float& Vector3::operator[](const int i) {
	return components[i];
}

const float& Vector3::operator[](const int i) const {
	return components[i];
}

Vector3& Vector3::operator*=(const float s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s) {
	s = 1.f / s;  // More performant to only do division once and then multiply
	*this *= s;
	return *this;
}

Vector3& Vector3::operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

void Vector3::normalize() {
	*this = this->normalized();
}

Vector3 Vector3::normalized() const {
	return *this / magnitude();
}

bool Vector3::is_near_zero() const {
	static const float s = 1e-16f;
	return squared_magnitude() < s;
}

void Vector3::clamp(float clamped_magnitude) {
	*this = this->clamped(clamped_magnitude);
}

Vector3 Vector3::clamped(float clamped_magnitude) const {
	auto current_magnitude = this->magnitude();
	if(current_magnitude < clamped_magnitude) return *this;

	return this->normalized() * clamped_magnitude;
}

float Vector3::squared_magnitude() const {
	return x * x + y * y + z * z;
}

float Vector3::magnitude() const {
	return std::sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::cross(const Vector3& v) const {
	return {y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x};
}

Vector3 Vector3::reflect(const Vector3& n) const {
	return *this - 2.f * this->dot(n) * n;
}

Vector3 Vector3::refract(const Vector3& n, float etai_over_etat) {
	auto cos_theta = fmin(dot(-n), 1.f);
	auto perp = etai_over_etat * (*this + cos_theta * n);
	auto parallel = -sqrt(fabs(1.f - perp.squared_magnitude())) * n;
	return perp + parallel;
}


float Vector3::dot(const Vector3& v) const {
	return x * v.x + y * v.y + z * v.z;
}

Vector3 operator*(Vector3 a, const Vector3& b) {
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	return a;
}

Vector3 operator*(Vector3 v, float s) {
	v *= s;
	return v;
}

Vector3 operator*(float s, Vector3 v) {
	return v * s;
}

Vector3 operator/(Vector3 v, float s) {
	v /= s;
	return v;
}

Vector3 operator+(Vector3 a, const Vector3& b) {
	a += b;
	return a;
}

Vector3 operator-(Vector3 a, const Vector3& b) {
	a -= b;
	return a;
}

Vector3 operator-(Vector3 v) {
	v *= -1.f;
	return v;
}

std::ostream& operator<<(std::ostream& out, const Vector3& v) {
	return out << v.x << ' ' << v.y << ' ' << v.z;
}
