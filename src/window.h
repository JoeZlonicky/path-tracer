#pragma once

#include <string>

class SDL_Window;
class SDL_Surface;

class Window {
public:
	~Window();

	bool init();
	bool load_image(std::string file_path);
	bool should_keep_open();
	void resize_window_to_image_size();
	void draw();

private:
	SDL_Window* _window = nullptr;
	SDL_Surface* _screen = nullptr;
	SDL_Surface* _image = nullptr;
};