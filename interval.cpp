#include "interval.h"

#include "utility.h"

Interval::Interval() : min(infinity), max(-infinity)
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

const static Interval empty { infinity, -infinity };
const static Interval universe{ -infinity, infinity };