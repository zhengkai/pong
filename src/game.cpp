#include "game.h"
#include "input.h"
#include <algorithm>

Game::Game(GameDep dep) : d(std::move(dep)) {
}

Game::~Game() {
}

void Game::parse(Input *in) {
	if (in->d.speed != 0) {
		int slv = d.entity->speedLevel + in->d.speed;
		slv = std::max(-3, std::min(3, slv));
		d.entity->speedLevel = slv;
		d.entity->speed = std::pow(2, slv);
	}
}
