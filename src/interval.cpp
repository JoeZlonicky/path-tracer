#include "interval.h"

#include "utility.h"

Interval::Interval() : min(Utility::infinity), max(-Utility::infinity)
{
}

Interval::Interval(double p_min, double p_max) : min(p_min), max(p_max)
{
}

void Interval::expand(double delta)
{
	*this = this->expanded(delta);
}

Interval Interval::expanded(double delta) const
{
	auto padding = delta / 2.0;
	return { min - padding, max + padding };
}

bool Interval::contains(double x) const
{
	return min <= x && x <= max;
}

bool Interval::surrounds(double x) const
{
	return min < x && x < max;
}

double Interval::clamp(double x) const
{
	if (x < min) return min;
	if (x > max) return max;

	return x;
}

const static Interval empty{ Utility::infinity, -Utility::infinity };
const static Interval universe{ -Utility::infinity, Utility::infinity };