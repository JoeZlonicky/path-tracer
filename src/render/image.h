#pragma once

#include <utility>
#include <vector>

#include "../math/vector_3.h"

class Image {
public:
	Image(int width, int height);

	void set_pixel(int x, int y, const Color& color);
	void set_pixel(int i, const Color& color);

	[[nodiscard]] const Color& get_pixel(int x, int y) const;
	[[nodiscard]] const Color& get_pixel(int i) const;
	[[nodiscard]] const std::vector<Color>& get_pixels() const;  // Row major

	[[nodiscard]] int get_width() const;
	[[nodiscard]] int get_height() const;
	[[nodiscard]] float get_aspect_ratio() const;
	[[nodiscard]] std::pair<int, int> i_to_coord(int i) const;

private:
	std::vector<Color> _pixels;
	int _width;
	int _height;
	int _size;
};