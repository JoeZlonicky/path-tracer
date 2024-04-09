#include "window.h"

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>


Window::~Window()
{
	if (_window != nullptr) SDL_DestroyWindow(_window);
	if (SDL_WasInit(SDL_INIT_VIDEO)) SDL_Quit();
}

bool Window::init(std::string window_name)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::clog << "Failed to initialize SDL2. Error: " << SDL_GetError() << std::endl;
		return false;
	}

	_window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1, 1, SDL_WINDOW_SHOWN);

	if (_window == nullptr) {
		std::clog << "Failed to create SDL2 window. Error: " << SDL_GetError() << std::endl;
		return false;
	}

	_screen = SDL_GetWindowSurface(_window);

	return true;
}

bool Window::load_image(std::string file_path)
{
	SDL_Surface* loaded_surface = IMG_Load(file_path.c_str());
	if (loaded_surface == nullptr) {
		std::clog << "Failed to load image at path '" << file_path << "'. Error: " << IMG_GetError() << std::endl;
		return false;
	}

	SDL_Surface* optimized_surface = SDL_ConvertSurface(loaded_surface, _screen->format, 0);
	if (optimized_surface == nullptr) {
		std::clog << "Failed to optimize image at path '" << file_path << "'. Error: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(loaded_surface);
		return false;
	}

	_image = optimized_surface;
	SDL_FreeSurface(loaded_surface);
	return true;
}

bool Window::should_keep_open()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) return false;
	}
	return true;
}

void Window::resize_window_to_image_size()
{
	if (_image == nullptr) {
		std::clog << "Unable to resize window to image size. No image set." << std::endl;
		return;
	}

	SDL_SetWindowSize(_window, _image->w, _image->h);
	SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	_screen = SDL_GetWindowSurface(_window);
}

void Window::draw()
{
	SDL_FillRect(_screen, nullptr, SDL_MapRGB(_screen->format, 0x00, 0x00, 0x00));
	if (_image) {
		SDL_BlitSurface(_image, nullptr, _screen, nullptr);
	}
	SDL_UpdateWindowSurface(_window);
}
