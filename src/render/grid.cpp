#include "grid.h"
#include "../config.hpp"
#include "../context/window.h"
#include <cmath>
#include <spdlog/spdlog.h>

void calcGrid(float winW, float winH, std::shared_ptr<context::Window> w) {

	float gs =
		std::floor(winW / cfgGridWF < winH / cfgGridHF ? winW / cfgGridWF
													   : winH / cfgGridHF);

	w->gridSize = gs;
	spdlog::info("gridSize = {}", gs);

	w->startX = std::floor((cfgWinW - (gs * cfgGridW)) / 2);
	w->startY = std::floor((cfgWinH - (gs * cfgGridH)) / 2);
	spdlog::info("w {}*{}={}", gs, cfgGridW, gs * cfgGridW);
	spdlog::info("h {}*{}={}", gs, cfgGridH, gs * cfgGridH);
	spdlog::info("startX = {}, startY = {}", w->startX, w->startY);
}
