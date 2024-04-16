#include "window.h"

#include <iostream>
#include <memory>
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_surface.h>
#include <SDL_video.h>
#include <string>

#include "../math/interval.h"
#include "../render/camera.h"
#include "user_interface.h"

namespace {
	// https://stackoverflow.com/questions/20070155/how-to-set-a-pixel-in-a-sdl-surface
	void set_pixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
		Uint32* target_pixel = (Uint32*)((Uint8*)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel);
		*target_pixel = pixel;
	}
}

Window::Window(std::string window_name, int width, int height) : _width(width), _height(height) {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::clog << "Failed to initialize SDL2. Error: " << SDL_GetError() << std::endl;
		return;
	}

	_window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if(_window == nullptr) {
		std::clog << "Failed to create SDL2 window. Error: " << SDL_GetError() << std::endl;
		return;
	}

	_renderer = SDL_CreateRenderer(_window, -1, 0);
	if(_renderer == nullptr) {
		std::clog << "Failed to create SDL2 renderer. Error: " << SDL_GetError() << std::endl;
		return;
	}
	_screen = SDL_GetWindowSurface(_window);
	_ui = std::make_unique<UserInterface>(_window, _renderer);

	_viewport_rect.x = 200;
	_viewport_rect.w = width - 200;
	_viewport_rect.h = height;
}

Window::~Window() {
	if(_render_texture != nullptr) SDL_DestroyTexture(_render_texture);
	if(_renderer != nullptr) SDL_DestroyRenderer(_renderer);
	if(_window != nullptr) SDL_DestroyWindow(_window);
	if(SDL_WasInit(0)) SDL_Quit();
}

void Window::set_camera(std::shared_ptr<Camera> camera) {
	_camera = camera;
	_ui->set_camera(camera);
}

bool Window::update() {

	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		_ui->process_event(&e);
		if(e.type == SDL_QUIT) {
			return false;
		}
	}

	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderClear(_renderer);
	SDL_RenderCopy(_renderer, _render_texture, nullptr, &_viewport_rect);
	_ui->update();
	check_for_new_render();
	SDL_RenderPresent(_renderer);
	return true;
}

void Window::check_for_new_render() {
	if(_camera->is_rendering()) return;

	auto render = _camera->get_render();
	if(render != _latest_render) {
		SDL_DestroyTexture(_render_texture);

		auto render_surface = SDL_CreateRGBSurface(0, render->get_width(), render->get_height(), 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
		SDL_LockSurface(render_surface);
		Interval intensity{0.000, 0.999};
		constexpr int max_value = 255;

		for(int y = 0; y < render->get_height(); ++y) {
			for(int x = 0; x < render->get_width(); ++x) {
				const auto& color = render->get_pixel(x, y);
				auto r = static_cast<Uint8>(max_value * intensity.clamped(color.x));
				auto g = static_cast<Uint8>(max_value * intensity.clamped(color.y));
				auto b = static_cast<Uint8>(max_value * intensity.clamped(color.z));
				Uint8 a = 255;
				Uint32 rgba = (r << 24) + (g << 16) + (b << 8) + a;
				set_pixel(render_surface, x, y, rgba);
			}
		}
		SDL_UnlockSurface(render_surface);

		_render_texture = SDL_CreateTextureFromSurface(_renderer, render_surface);
		SDL_FreeSurface(render_surface);

		_latest_render = render;
	}
}
