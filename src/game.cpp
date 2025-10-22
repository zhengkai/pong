#include "game.h"
#include "config.hpp"
#include <algorithm>

static std::string speedMsg = "Speed Level: ";

Game::Game(GameDep dep) : d(std::move(dep)) {
}

Game::~Game() {
}

bool Game::parse() {

	if (d.input->quit) {
		return false;
	}

	if (d.input->speed != 0) {
		int slv = d.entity->speedLevel + d.input->speed;
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

	return true;
}
