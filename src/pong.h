#pragma once

#include "input.h"
#include "time.hpp"

class Pong {

private:
	bool isRunning;
	Input *input;
	Time *t;

public:
	Pong();
	~Pong();
	void loop();
	void run();
};
