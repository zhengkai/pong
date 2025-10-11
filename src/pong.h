#pragma once

#include "input.h"
#include "physics/physics.h"
#include "sdl.h"
#include "time.hpp"

class Pong {

private:
	bool stop;
	Input *input;
	Time *t;
	sdl *s;
	Physics *p;

public:
	Pong();
	~Pong();
	void loop(int cnt);
	void run();

private:
	void sdlBg();
};
