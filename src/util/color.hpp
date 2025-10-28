#pragma once

#include <SDL3/SDL_pixels.h>

namespace util {

inline SDL_Color lighten(const SDL_Color sc, float amount = 0.2f) {
	SDL_Color result;
	result.r = static_cast<Uint8>(sc.r + (255 - sc.r) * amount);
	result.g = static_cast<Uint8>(sc.g + (255 - sc.g) * amount);
	result.b = static_cast<Uint8>(sc.b + (255 - sc.b) * amount);
	result.a = sc.a;
	return result;
};

inline SDL_Color invert(const SDL_Color sc) {
	SDL_Color result;
	result.r = static_cast<Uint8>(255 - sc.r);
	result.g = static_cast<Uint8>(255 - sc.g);
	result.b = static_cast<Uint8>(255 - sc.b);
	result.a = sc.a;
	return result;
};

}; // namespace util
