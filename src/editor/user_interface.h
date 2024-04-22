#pragma once

#include <imgui.h>
#include <memory>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <string>

#include "../render/camera.h"
#include "window.h"

class UserInterface {
public:
	UserInterface(Window& window, std::function<std::string()> save_func);
	~UserInterface();

	void process_event(const SDL_Event* e);
	void update();

	void set_camera(std::shared_ptr<Camera> camera);

	[[nodiscard]] int get_current_scene_selection() const;
	[[nodiscard]] int get_current_material_selection() const;

	const int width = 400;

private:
	std::function<std::string()> _save_func;
	std::shared_ptr<Camera> _camera;
	Window& _window;
	ImGuiIO* _io = nullptr;

	std::string _save_name = "";
	int _current_scene_selection = 0;
	int _current_material_selection = 0;
};