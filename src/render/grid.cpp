#include "grid.h"
#include "../config.hpp"
#include "../context/window.h"
#include <cmath>
#include <spdlog/spdlog.h>

void calcGrid(int winW, int winH, std::shared_ptr<context::Window> w) {

	float ww = static_cast<float>(winW);
	float wh = static_cast<float>(winH);

	float gs = std::floor(
		ww / cfgGridWF < wh / cfgGridHF ? ww / cfgGridWF : wh / cfgGridHF);

	w->gridSize = gs;
	spdlog::info("gridSize = {}, win = {}x{}", gs, winW, winH);

	w->startX = std::floor((winW - (gs * cfgGridW)) / 2);
	w->startY = std::floor((winH - (gs * cfgGridH)) / 2);
	spdlog::info("w {}*{}={}", gs, cfgGridW, gs * cfgGridW);
	spdlog::info("h {}*{}={}", gs, cfgGridH, gs * cfgGridH);
	spdlog::info("startX = {}, startY = {}", w->startX, w->startY);
}
