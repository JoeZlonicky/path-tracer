#pragma once

class Interval {
public:
	double min;
	double max;

	Interval();
	Interval(double p_min, double p_max);
	Interval(const Interval& a, const Interval& b);

	void expand(double delta);
	Interval expanded(double delta) const;

	double size() const;
	bool contains(double x) const;
	bool surrounds(double x) const;
	double clamp(double x) const;

	static Interval empty;
	static Interval universe;
};