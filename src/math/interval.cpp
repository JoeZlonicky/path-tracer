#include "interval.h"

#include "math_utility.h"

Interval Interval::empty{MathUtility::infinity, -MathUtility::infinity};
Interval Interval::universe{-MathUtility::infinity, MathUtility::infinity};

Interval::Interval() : min(MathUtility::infinity), max(-MathUtility::infinity) {
}

Interval::Interval(double p_min, double p_max) : min(p_min), max(p_max) {
}

Interval::Interval(const Interval& a, const Interval& b) {
	min = a.min <= b.min ? a.min : b.min;
	max = a.max >= b.max ? a.max : b.max;
}

void Interval::expand(double delta) {
	*this = this->expanded(delta);
}

Interval Interval::expanded(double delta) const {
	auto padding = delta / 2.0;
	return {min - padding, max + padding};
}

double Interval::clamped(double x) const {
	if(x < min) return min;
	if(x > max) return max;

	return x;
}

double Interval::size() const {
	return max - min;
}

bool Interval::contains(double x) const {
	return min <= x && x <= max;
}

bool Interval::surrounds(double x) const {
	return min < x && x < max;
}