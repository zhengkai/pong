#pragma once

#include "context/entity.h"
#include "input.h"
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
	Input *input;
	Time *t;
	sdl *s;
	std::vector<std::unique_ptr<Region>> region;

public:
	bool stop;
	Pong();
	~Pong();
	void loop();
	void run();
	bool init();

private:
	void sdlBg();
	void loopEvent();
};
