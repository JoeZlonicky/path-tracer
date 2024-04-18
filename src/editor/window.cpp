#include "window.h"

#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <stdexcept>
#include <string>


Window::Window(std::string window_name, int width, int height) : _width(width), _height(height) {
	_window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (_window == nullptr) {
		throw std::runtime_error("Failed to create SDL2 window");
	}

	_renderer = SDL_CreateRenderer(_window, -1, 0);
	if (_renderer == nullptr) {
		SDL_DestroyWindow(_window);
		throw std::runtime_error("Failed to create SDL2 renderer");
	}

	_screen = SDL_GetWindowSurface(_window);
}

Window::~Window() {
	if (_renderer != nullptr) SDL_DestroyRenderer(_renderer);
	if (_window != nullptr) SDL_DestroyWindow(_window);
}

void Window::clear()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderClear(_renderer);
}

void Window::refresh()
{
	SDL_RenderPresent(_renderer);
}

void Window::resized(int width, int height)
{
	_width = width;
	_height = height;
}

void Window::draw_texture(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest)
{
	SDL_RenderCopy(_renderer, texture, src, dest);
}

int Window::get_width() const
{
	return _width;
}

int Window::get_height() const
{
	return _height;
}

SDL_Window* Window::get_sdl_window()
{
	return _window;
}

SDL_Renderer* Window::get_sdl_renderer()
{
	return _renderer;
}
