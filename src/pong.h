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
	std::thread bgThread;
	PongDeps d;
	Input *input;
	Time *t;
	sdl *s;
	std::vector<std::unique_ptr<Region>> region;
	int serial;

public:
	bool stop;
	Pong();
	~Pong();
	void loop();
	void run();
	bool init();

private:
	void sdlBg();

	void sdlBgStep();
	void startBg();
	void stopBg();
};
