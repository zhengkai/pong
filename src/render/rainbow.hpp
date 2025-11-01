#pragma once

#include "../config.hpp"
#include "../context/ball.h"
#include "../context/window.h"
#include "../util/hct.hpp"
#include <SDL3/SDL_render.h>
#include <memory>

class Rainbow {

private:
	SDL_Renderer *r;

public:
	Rainbow(SDL_Renderer *r) : r(r) {};
	~Rainbow() {};

	void render(std::vector<std::shared_ptr<context::BallGroup>> bl,
		std::shared_ptr<context::Window> window) {

		auto rainbow = util::Rainbow(10);

		float size = window->gridSize * cfgPaddingTop;

		SDL_FRect rect;
		rect.w = window->gridSize * cfgPaddingTop * 0.875f;
		rect.h = rect.w;
		rect.y = window->startY - size;

		float i = 0.0f;
		for (const auto &b : bl) {
			rect.x = window->startX + size * i;
			i++;
			double tone = b->hit ? 80.0 : 60.0;
			auto c = util::HCT(b->hue, 80, tone).ToColor();
			SDL_SetRenderDrawColor(r, c.r, c.g, c.b, 255);
			SDL_RenderFillRect(r, &rect);
		}
	};
};
