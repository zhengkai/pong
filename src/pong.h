#pragma once

#include "context/entity.h"
#include "context/window.h"
#include "game.h"
#include "region.hpp"
#include "sdl.h"
#include "time.hpp"

struct PongDeps {
	std::shared_ptr<context::Entity> entity;
	std::shared_ptr<context::Window> window;
};

class Pong {

private:
	PongDeps d;
	std::unique_ptr<Time> t;
	sdl *s;
	Game *g;
	std::vector<std::unique_ptr<Region>> region;

private:
	std::vector<context::Brick> genBrick();

public:
	bool stop;
	Pong();
	~Pong();
	void loop();
	void run();
	bool init();
	bool initBrick();
};
