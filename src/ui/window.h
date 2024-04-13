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

	void set_camera(std::shared_ptr<Camera> camera);
	bool init_was_successful();
	bool update();

private:
	void check_for_new_render();

	std::shared_ptr<Camera> _camera;
	std::shared_ptr<Image> _latest_render;

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	SDL_Surface* _screen = nullptr;
	SDL_Texture* _render_texture = nullptr;
	SDL_Rect _viewport_rect;
	std::unique_ptr<UserInterface> _ui;

	bool _init_was_successful = false;
	int _width;
	int _height;
};