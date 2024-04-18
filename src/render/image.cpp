#include "image.h"

#include <vector>

#include "../math/vector_3.h"

Image::Image(int width, int height) : _width(width), _height(height) {
	_size = _width * _height;
	_pixels.resize(_size);
}

void Image::set_pixel(int x, int y, const Color& color) {
	auto i = y * _width + x;
	_pixels[i] = color;
}

void Image::set_pixel(int i, const Color& color)
{
	_pixels[i] = color;
}

const Color& Image::get_pixel(int x, int y) const {
	auto i = y * _width + x;
	return _pixels[i];
}

const Color& Image::get_pixel(int i) const
{
	return _pixels[i];
}

const std::vector<Color>& Image::get_pixels() const {
	return _pixels;
}

int Image::get_width() const {
	return _width;
}

int Image::get_height() const {
	return _height;
}

float Image::get_aspect_ratio() const
{
	return static_cast<float>(_width) / static_cast<float>(_height);
}

std::pair<int, int> Image::i_to_coord(int i) const
{
	auto x = i % _width;
	auto y = i / _width;
	return { x, y };
}
