#include "grid.h"
#include "../config.hpp"
#include "layout.hpp"
#include "spdlog/spdlog.h"
#include <cmath>
#include <vector>

Grid::Grid(SDL_Renderer *r, Layout *layout) : r(r) {
	calcGrid(layout);
}

Grid::~Grid() {
	r = nullptr;
}

void Grid::draw(const std::vector<bool> &li) {
	int index = 0;
	for (auto cell : li) {
		if (cell) {
			int x = index % cfgGridW;
			int y = index / cfgGridW;

			SDL_FRect rect;
			rect.x = startX + x * gridSize;
			rect.y = startY + y * gridSize;
			rect.w = gridSize;
			rect.h = gridSize;
			SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
			SDL_RenderFillRect(r, &rect);
		}
		++index;
	}
}

void Grid::calcGrid(Layout *layout) {
	gridSize =
		std::floor(static_cast<float>(cfgWinW / cfgGridW < cfgWinH / cfgGridH)
				? cfgWinW / cfgGridW
				: cfgWinH / cfgGridH);
	spdlog::info("gridSize = {}", gridSize);

	startX = std::floor((cfgWinW - (gridSize * cfgGridW)) / 2);
	startY = std::floor((cfgWinH - (gridSize * cfgGridH)) / 2);
	spdlog::info("w {}*{}={}", gridSize, cfgGridW, gridSize * cfgGridW);
	spdlog::info("h {}*{}={}", gridSize, cfgGridH, gridSize * cfgGridH);
	spdlog::info("startX = {}, startY = {}", startX, startY);
}
