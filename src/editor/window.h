#pragma once

#include <imgui.h>
#include <memory>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_video.h>
#include <string>

#include "../render/camera.h"

class Window {
public:
	Window(std::string window_name, int width, int height);
	~Window();

	Window(const Window& window) = delete;
	Window& operator=(const Window& window) = delete;

	void clear();
	void refresh();

	void resized(int width, int height);
	void draw_texture(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest);

	[[nodiscard]] int get_width() const;
	[[nodiscard]] int get_height() const;
	[[nodiscard]] SDL_Window* get_sdl_window();
	[[nodiscard]] SDL_Renderer* get_sdl_renderer();

private:
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	SDL_Surface* _screen = nullptr;

	int _width;
	int _height;
};