#pragma once

#include "input.h"
#include "sdl.h"
#include "time.hpp"

class Pong {

private:
	bool isRunning;
	Input *input;
	Time *t;
	sdl *s;

public:
	Pong();
	~Pong();
	void loop();
	void run();

private:
	void sdlBg();
};
