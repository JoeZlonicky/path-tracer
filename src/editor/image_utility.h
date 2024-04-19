#pragma once

#include <cmath>
#include <SDL_image.h>

namespace ImageUtility {
	// https://stackoverflow.com/questions/20070155/how-to-set-a-pixel-in-a-sdl-surface
	inline void set_sdl_surface_pixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
		Uint32* target_pixel = (Uint32*)((Uint8*)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel);
		*target_pixel = pixel;
	}

	inline float linear_to_gamma(float s) {
		if (s > 0.f) {
			return sqrtf(s);
		}
		return 0.f;
	}

	inline void save_sdl_surface_as_png(SDL_Surface* surface) {
		IMG_SavePNG(surface, "output.png");
	}
}