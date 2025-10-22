#pragma once

#include "context/entity.h"
#include "context/input.hpp"
#include "context/window.h"

struct GameDep {
	std::shared_ptr<context::Entity> entity;
	std::shared_ptr<context::Window> window;
	std::shared_ptr<context::Input> input;
};

class Game {

private:
	GameDep d;

public:
	Game(GameDep dep);
	~Game();

	bool parse();
};
