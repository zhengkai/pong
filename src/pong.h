#pragma once

#include "context/entity.h"
#include "context/input.hpp"
#include "context/window.h"
#include "game.h"
#include "region.hpp"
#include "sdl.h"
#include "time.hpp"

struct PongDeps {
	std::shared_ptr<context::Entity> entity;
	std::shared_ptr<context::Window> window;
	std::shared_ptr<context::Input> input;
};

class Pong {

private:
	PongDeps d;
	Time *t;
	sdl *s;
	Game *g;
	std::vector<std::unique_ptr<Region>> region;

public:
	bool stop;
	Pong();
	~Pong();
	void loop();
	void run();
	bool init();
};
