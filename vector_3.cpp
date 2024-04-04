#include "vector_3.h"

#include <cmath>
#include "utility.h"

Vector3::Vector3() : x(0.f), y(0.f), z(0.f) {}

Vector3::Vector3(double value) : x(value), y(value), z(value) {}

Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

Vector3 Vector3::random()
{
    return Vector3(random_normalized(), random_normalized(), random_normalized());
}

Vector3 Vector3::random(double min, double max)
{
    return Vector3(random_range(min, max), random_range(min, max), random_range(min, max));
}

double& Vector3::operator[](const int i) {
    return components[i];
}

const double& Vector3::operator[](const int i) const {
    return components[i];
}

Vector3& Vector3::operator*=(const double s) {
    x *= s;
    y *= s;
    z *= s;
    return *this;
}

Vector3& Vector3::operator/=(double s) {
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
    *this /= magnitude();
}

Vector3 Vector3::normalized() const {
    return *this / magnitude();
}

bool Vector3::is_near_zero() const
{
    static const double s = 1e-16;
    return squared_magnitude() < s;
}

void Vector3::clamp(double clamped_magnitude) {
    double current_magnitude = this->magnitude();
    if (current_magnitude < clamped_magnitude) return;

    normalize();
    *this *= clamped_magnitude;
}

Vector3 Vector3::clamped(double clamped_magnitude) const {
    double current_magnitude = this->magnitude();
    if (current_magnitude < clamped_magnitude) return *this;

    return this->normalized() * clamped_magnitude;
}

double Vector3::squared_magnitude() const
{
    return x * x + y * y + z * z;
}

double Vector3::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::cross(const Vector3& v) const {
    return { y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x };
}

Vector3 Vector3::reflect(const Vector3& n) const
{
    return *this - 2.0 * this->dot(n) * n;
}

Vector3 Vector3::refract(const Vector3& n, double etai_over_etat)
{
    auto cos_theta = fmin(dot(-n), 1.0);
    auto perp = etai_over_etat * (*this + cos_theta * n);
    auto parallel = -sqrt(fabs(1.0 - perp.squared_magnitude())) * n;
    return perp + parallel;
}


double Vector3::dot(const Vector3& v) const {
    return x * v.x + y * v.y + z * v.z;
}

Vector3 operator*(Vector3 a, const Vector3& b)
{
    a.x *= b.x;
    a.y *= b.y;
    a.z *= b.z;
    return a;
}

Vector3 operator*(Vector3 v, double s) {
    v *= s;
    return v;
}

Vector3 operator*(double s, Vector3 v)
{
    return v * s;
}

Vector3 operator/(Vector3 v, double s) {
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

std::ostream& operator<<(std::ostream& out, const Vector3& v)
{
    return out << v.x << ' ' << v.y << ' ' << v.z;
}
