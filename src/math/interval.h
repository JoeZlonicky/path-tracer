#pragma once

class Interval {
public:
	double min;
	double max;

	Interval();
	Interval(double p_min, double p_max);
	Interval(const Interval& a, const Interval& b);

	void expand(double delta);
	[[nodiscard]] Interval expanded(double delta) const;

	[[nodiscard]] double clamped(double x) const;
	[[nodiscard]] double size() const;
	[[nodiscard]] bool contains(double x) const;
	[[nodiscard]] bool surrounds(double x) const;

	static Interval empty;
	static Interval universe;
	static Interval unit;
};