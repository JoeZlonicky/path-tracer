#pragma once

#include <cmath>
#include <ctime>
#include <SDL_image.h>
#include <string>

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

	inline std::string save_sdl_surface_as_png(SDL_Surface* surface) {
		static int n = 1;

		auto t = std::time(0);
		auto filename = "output_" + std::to_string(t) + "_" + std::to_string(n++) + ".png";

		IMG_SavePNG(surface, filename.c_str());
		return filename;
	}
}