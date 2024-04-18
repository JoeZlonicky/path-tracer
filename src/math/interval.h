#pragma once

class Interval {
public:
	float min;
	float max;

	Interval();
	Interval(float p_min, float p_max);
	Interval(const Interval& a, const Interval& b);

	void expand(float delta);
	[[nodiscard]] Interval expanded(float delta) const;

	[[nodiscard]] float clamped(float x) const;
	[[nodiscard]] float size() const;
	[[nodiscard]] bool contains(float x) const;
	[[nodiscard]] bool surrounds(float x) const;

	static Interval empty;
	static Interval universe;
	static Interval unit;
};