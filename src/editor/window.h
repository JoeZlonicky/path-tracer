#pragma once

#include <imgui.h>
#include <memory>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_video.h>
#include <string>

#include "../render/camera.h"
#include "user_interface.h"

class Window {
public:
	Window(std::string window_name, int width, int height);
	~Window();

	Window(const Window& window) = delete;
	Window& operator=(const Window& window) = delete;

	void set_camera(std::shared_ptr<Camera> camera);
	bool update();

private:
	void check_for_new_render();

	std::shared_ptr<Camera> _camera = nullptr;
	std::shared_ptr<Image> _latest_render = nullptr;
	std::unique_ptr<UserInterface> _ui = nullptr;

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	SDL_Surface* _screen = nullptr;
	SDL_Texture* _render_texture = nullptr;
	SDL_Rect _viewport_rect{};

	int _width;
	int _height;
};