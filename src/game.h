#pragma once

#include "context/entity.h"
#include "context/window.h"
#include "input.h"

struct GameDep {
	std::shared_ptr<context::Entity> entity;
	std::shared_ptr<context::Window> window;
};

class Game {

private:
	GameDep d;

public:
	Game(GameDep dep);
	~Game();

	void parse(Input *in);
};
