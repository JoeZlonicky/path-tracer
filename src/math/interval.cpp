#include "interval.h"

#include "math_utility.h"

Interval Interval::empty{MathUtility::infinity, -MathUtility::infinity};
Interval Interval::universe{-MathUtility::infinity, MathUtility::infinity};
Interval Interval::unit{0.f, 1.f};

Interval::Interval() : min(MathUtility::infinity), max(-MathUtility::infinity) {
}

Interval::Interval(float p_min, float p_max) : min(p_min), max(p_max) {
}

Interval::Interval(const Interval& a, const Interval& b) {
	min = a.min <= b.min ? a.min : b.min;
	max = a.max >= b.max ? a.max : b.max;
}

void Interval::expand(float delta) {
	*this = this->expanded(delta);
}

Interval Interval::expanded(float delta) const {
	auto padding = delta / 2.f;
	return {min - padding, max + padding};
}

float Interval::clamped(float x) const {
	if(x < min) return min;
	if(x > max) return max;

	return x;
}

float Interval::size() const {
	return max - min;
}

bool Interval::contains(float x) const {
	return min <= x && x <= max;
}

bool Interval::surrounds(float x) const {
	return min < x && x < max;
}