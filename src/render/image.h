#pragma once

#include <vector>

#include "../math/vector_3.h"

class Image {
public:
	Image(int width, int height);

	void set_pixel(int x, int y, const Color& color);
	[[nodiscard]] const Color& get_pixel(int x, int y) const;
	[[nodiscard]] const std::vector<Color>& get_pixels() const;  // Row major

	[[nodiscard]] int get_width() const;
	[[nodiscard]] int get_height() const;

private:
	std::vector<Color> _pixels;
	int _width;
	int _height;
};