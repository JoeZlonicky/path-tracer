#pragma once

#include <vector>

#include "vector_3.h"

class Image {
public:
	Image(int width, int height);

	void set_pixel(int x, int y, const Color& color);
	const Color& get_pixel(int x, int y) const;
	const std::vector<Color>& get_pixels() const;  // Row major

	int get_width() const;
	int get_height() const;

private:
	std::vector<Color> _pixels;
	int _width;
	int _height;
};