#pragma once

#include "context/entity.h"
#include "input.h"
#include "region.hpp"
#include "sdl.h"
#include "time.hpp"

struct PongDeps {
	std::shared_ptr<context::Entity> entity;
};

class Pong {

private:
	PongDeps d;
	bool stop;
	Input *input;
	Time *t;
	sdl *s;
	std::vector<std::unique_ptr<Region>> region;

public:
	Pong();
	~Pong();
	void loop(int cnt);
	void run();
	void init();

private:
	void sdlBg();
};
