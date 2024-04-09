#pragma once

class Interval {
public:
	double min;
	double max;

	Interval();
	Interval(double p_min, double p_max);

	void expand(double delta);
	Interval expanded(double delta) const;

	bool contains(double x) const;
	bool surrounds(double x) const;
	double clamp(double x) const;

	static const Interval empty;
	static const Interval universe;
};