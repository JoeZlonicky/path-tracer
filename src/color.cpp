#include "color.h"

#include "interval.h"
#include "math.h"

namespace {
	inline double linear_to_gamma(double linear) {
		return sqrt(linear);
	}
}

void write_color_256(std::ostream& out, Color pixel_color)
{
	auto r = pixel_color.x;
	auto g = pixel_color.y;
	auto b = pixel_color.z;

	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	static const Interval intensity(0.000, 0.999);
	static const int max_value = 256;

	out << static_cast<int>(max_value * intensity.clamp(r)) << ' '
		<< static_cast<int>(max_value * intensity.clamp(g)) << ' '
		<< static_cast<int>(max_value * intensity.clamp(b)) << '\n';
}
