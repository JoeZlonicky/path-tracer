#include "image.h"

Image::Image(int width, int height) : _width(width), _height(height), _pixels{ static_cast<unsigned int>(width * height) }
{
}

void Image::set_pixel(int x, int y, const Color& color)
{
	auto i = y * _width + x;
	_pixels[i] = color;
}

const Color& Image::get_pixel(int x, int y) const
{
	auto i = y * _width + x;
	return _pixels[i];
}

const std::vector<Color>& Image::get_pixels() const
{
	return _pixels;
}

int Image::get_width() const
{
	return _width;
}

int Image::get_height() const
{
	return _height;
}
