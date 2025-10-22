#include "game.h"
#include "config.hpp"
#include "input.h"
#include <algorithm>

static std::string speedMsg = "Speed Level: ";

Game::Game(GameDep dep) : d(std::move(dep)) {
}

Game::~Game() {
}

void Game::parse(Input *in) {
	if (in->d.speed != 0) {
		int slv = d.entity->speedLevel + in->d.speed;
		slv = std::max(-cfgSpeedLevelMax, std::min(cfgSpeedLevelMax, slv));
		d.entity->speedLevel = slv;
		d.entity->speed = std::pow(2, slv);

		context::ControlMsg *cm = d.window->controlMsg;
		if (cm == nullptr) {
			cm = new context::ControlMsg();
			d.window->controlMsg = cm;
		}

		std::string m = std::to_string((int)std::pow(2, std::abs(slv))) + "x";
		if (slv < 0) {
			m = "1/" + m;
		}
		cm->msg = speedMsg + m;
		cm->expireSerial = d.window->serial + cfgFPS * 2;
	}
}
