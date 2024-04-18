#pragma once

#include <imgui.h>
#include <memory>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include "../render/camera.h"
#include "window.h"

class UserInterface {
public:
	UserInterface(Window& window);
	~UserInterface();

	void process_event(const SDL_Event* e);
	void update();

	void set_camera(std::shared_ptr<Camera> camera);

	const int width = 400;

private:
	std::shared_ptr<Camera> _camera;
	Window& _window;
	ImGuiIO* _io = nullptr;
};