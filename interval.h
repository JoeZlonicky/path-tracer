#pragma once

class Interval {
public:
	double min;
	double max;

	Interval();
	Interval(double p_min, double p_max);

	bool contains(double x) const;
	bool surrounds(double x) const;

	static const Interval empty;
	static const Interval universe;
};