#include "editor.h"

#include <memory>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_surface.h>

#include "../math/interval.h"
#include "../render/camera.h"
#include "window.h"

namespace {
	// https://stackoverflow.com/questions/20070155/how-to-set-a-pixel-in-a-sdl-surface
	void set_pixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
		Uint32* target_pixel = (Uint32*)((Uint8*)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel);
		*target_pixel = pixel;
	}
}

Editor::Editor(Window& window) : _window(window), _ui{ _window }
{
	update_viewport_size_and_pos();
}

void Editor::update()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			_should_keep_open = false;
		}
		else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
			_window.resized(e.window.data1, e.window.data2);
			update_viewport_size_and_pos();
		}
		_ui.process_event(&e);
	}

	check_for_new_render();
}

void Editor::draw()
{
	_ui.update();
	_window.draw_texture(_render_texture, nullptr, &_viewport_rect);
}

bool Editor::should_keep_open() const
{
	return _should_keep_open;
}

void Editor::set_camera(std::shared_ptr<Camera> camera)
{
	_camera = camera;
	_ui.set_camera(camera);
}

void Editor::check_for_new_render() {
	if (_camera->is_rendering()) return;

	auto render = _camera->get_render();
	if (render == _latest_render) return;

	_latest_render = render;
	update_render();
}

void Editor::update_render()
{
	auto render_surface = SDL_CreateRGBSurface(0, _latest_render->get_width(), _latest_render->get_height(), 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	SDL_LockSurface(render_surface);
	Interval intensity{ 0.f, 0.999f };
	constexpr int max_value = 255;

	for (int y = 0; y < _latest_render->get_height(); ++y) {
		for (int x = 0; x < _latest_render->get_width(); ++x) {
			const auto& color = _latest_render->get_pixel(x, y);
			auto r = static_cast<Uint8>(max_value * intensity.clamped(color.x));
			auto g = static_cast<Uint8>(max_value * intensity.clamped(color.y));
			auto b = static_cast<Uint8>(max_value * intensity.clamped(color.z));
			Uint8 a = 255;
			Uint32 rgba = (r << 24) + (g << 16) + (b << 8) + a;
			set_pixel(render_surface, x, y, rgba);
		}
	}
	SDL_UnlockSurface(render_surface);

	SDL_DestroyTexture(_render_texture);
	_render_texture = SDL_CreateTextureFromSurface(_window.get_sdl_renderer(), render_surface);

	SDL_FreeSurface(render_surface);
	update_viewport_size_and_pos();
}

void Editor::update_viewport_size_and_pos()
{
	_viewport_rect.x = _ui.width;
	_viewport_rect.w = _window.get_width() - _ui.width;
	if (_latest_render == nullptr) {
		_viewport_rect.h = _window.get_height();
		return;
	}

	_viewport_rect.h = static_cast<int>(_viewport_rect.w / _latest_render->get_aspect_ratio());
	_viewport_rect.y = (_window.get_height() - _viewport_rect.h) / 2;
}
