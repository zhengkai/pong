#include "grid.h"
#include "../config.hpp"
#include "layout.hpp"
#include <cmath>
#include <spdlog/spdlog.h>
#include <vector>

Grid::Grid(SDL_Renderer *r) : r(r) {
	calcGrid();
}

Grid::~Grid() {
	r = nullptr;
}

void Grid::draw(const std::vector<bool> &li) {

	Layout &layout = Layout::instance();
	int index = 0;
	for (auto cell : li) {
		if (cell) {
			int x = index % cfgGridW;
			int y = index / cfgGridW;

			SDL_FRect rect;
			rect.x = layout.startX + x * layout.gridSize;
			rect.y = layout.startY + y * layout.gridSize;
			rect.w = layout.gridSize;
			rect.h = layout.gridSize;
			SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
			SDL_RenderFillRect(r, &rect);
		}
		++index;
	}
}

void Grid::calcGrid() {

	Layout &layout = Layout::instance();

	layout.gridSize =
		std::floor(static_cast<float>(cfgWinW / cfgGridW < cfgWinH / cfgGridH)
				? cfgWinW / cfgGridW
				: cfgWinH / cfgGridH);
	spdlog::info("gridSize = {}", layout.gridSize);

	layout.startX = std::floor((cfgWinW - (layout.gridSize * cfgGridW)) / 2);
	layout.startY = std::floor((cfgWinH - (layout.gridSize * cfgGridH)) / 2);
	spdlog::info(
		"w {}*{}={}", layout.gridSize, cfgGridW, layout.gridSize * cfgGridW);
	spdlog::info(
		"h {}*{}={}", layout.gridSize, cfgGridH, layout.gridSize * cfgGridH);
	spdlog::info("startX = {}, startY = {}", layout.startX, layout.startY);
}
