#pragma once

#include <imgui.h>
#include <memory>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include "../render/camera.h"

class UserInterface {
public:
	UserInterface(SDL_Window* window, SDL_Renderer* renderer);
	~UserInterface();

	void process_event(const SDL_Event* e);
	void update();

	void set_camera(std::shared_ptr<Camera> camera);

	const int width = 400;

private:
	std::shared_ptr<Camera> _camera;
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	ImGuiIO* _io = nullptr;
};