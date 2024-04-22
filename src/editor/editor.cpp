#include "editor.h"

#include <memory>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_surface.h>

#include "../math/interval.h"
#include "../render/camera.h"
#include "image_utility.h"
#include "window.h"


Editor::Editor(Window& window) : _window(window), _ui{ _window, [this]() -> std::string { return this->save_render(); } }
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

std::string Editor::save_render()
{
	if (_render_surface == nullptr) return "";
	return ImageUtility::save_sdl_surface_as_png(_render_surface);
}

void Editor::check_for_new_render() {
	if (_camera->is_rendering() || _camera->get_n_pixel_renders_remaining() == -1) return;

	auto render = _camera->get_render();
	if (render == _latest_render) return;

	_latest_render = render;
	update_render();
}

void Editor::update_render()
{
	SDL_FreeSurface(_render_surface);

	_render_surface = SDL_CreateRGBSurface(0, _latest_render->get_width(), _latest_render->get_height(), 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	SDL_LockSurface(_render_surface);
	Interval intensity{ 0.f, 0.999f };
	constexpr int max_value = 255;

	for (int y = 0; y < _latest_render->get_height(); ++y) {
		for (int x = 0; x < _latest_render->get_width(); ++x) {
			const auto& color = _latest_render->get_pixel(x, y);
			auto r = ImageUtility::linear_to_gamma(color.x);
			auto g = ImageUtility::linear_to_gamma(color.y);
			auto b = ImageUtility::linear_to_gamma(color.z);

			auto r_byte = static_cast<Uint8>(max_value * intensity.clamped(r));
			auto g_byte = static_cast<Uint8>(max_value * intensity.clamped(g));
			auto b_byte = static_cast<Uint8>(max_value * intensity.clamped(b));

			Uint8 a = 255;
			Uint32 rgba = (r_byte << 24) + (g_byte << 16) + (b_byte << 8) + a;
			ImageUtility::set_sdl_surface_pixel(_render_surface, x, y, rgba);
		}
	}
	SDL_UnlockSurface(_render_surface);

	SDL_DestroyTexture(_render_texture);
	_render_texture = SDL_CreateTextureFromSurface(_window.get_sdl_renderer(), _render_surface);

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
