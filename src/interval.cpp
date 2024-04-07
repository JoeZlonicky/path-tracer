#include "interval.h"

#include "utility.h"

Interval::Interval() : min(Utility::infinity), max(-Utility::infinity)
{
}

Interval::Interval(double p_min, double p_max) : min(p_min), max(p_max)
{
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