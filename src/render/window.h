#pragma once

#include <SDL_surface.h>
#include <SDL_video.h>
#include <string>

class Window {
public:
	~Window();

	bool init(std::string window_name);
	bool load_image(std::string file_path);
	[[nodiscard]] bool should_keep_open();
	void resize_window_to_image_size();
	void draw();

private:
	SDL_Window* _window = nullptr;
	SDL_Surface* _screen = nullptr;
	SDL_Surface* _image = nullptr;
};