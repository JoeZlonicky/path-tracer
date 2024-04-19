#pragma once

#include <memory>
#include <SDL_rect.h>
#include <SDL_render.h>

#include "../render/camera.h"
#include "../render/image.h"
#include "user_interface.h"
#include "window.h"

class Editor {
public:
	Editor(Window& window);

	void update();
	void draw();

	[[nodiscard]] bool should_keep_open() const;

	void set_camera(std::shared_ptr<Camera> camera);
	void save_render();

private:
	void check_for_new_render();
	void update_render();
	void update_viewport_size_and_pos();

	Window& _window;
	UserInterface _ui;
	std::shared_ptr<Camera> _camera = nullptr;
	std::shared_ptr<Image> _latest_render = nullptr;

	SDL_Surface* _render_surface = nullptr;  // Used for saving
	SDL_Texture* _render_texture = nullptr;  // User for rendering
	SDL_Rect _viewport_rect{};

	bool _should_keep_open = true;
};