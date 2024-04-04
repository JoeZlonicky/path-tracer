#include "color.h"

#include "interval.h"

void write_color_256(std::ostream& out, Color pixel_color, int samples_per_pixel)
{
	auto r = pixel_color.x;
	auto g = pixel_color.y;
	auto b = pixel_color.z;

	auto scale = 1.0 / samples_per_pixel;
	r *= scale;
	g *= scale;
	b *= scale;

	static const Interval intensity(0.000, 0.999);
	static const int max_value = 256;

	out << static_cast<int>(max_value * intensity.clamp(r)) << ' '
		<< static_cast<int>(max_value * intensity.clamp(g)) << ' '
		<< static_cast<int>(max_value * intensity.clamp(b)) << '\n';
}
