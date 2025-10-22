#pragma once

#include "context/entity.h"
#include "input.h"

struct GameDep {
	std::shared_ptr<context::Entity> entity;
};

class Game {

private:
	GameDep d;

public:
	Game(GameDep dep);
	~Game();

	void parse(Input *in);
};
