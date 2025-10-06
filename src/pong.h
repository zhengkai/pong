#pragma once

#include "input.h"

bool runPong(int argc, char *argv[]);

class Pong {

private:
	Input *input;

public:
	Pong();
	~Pong();
	void run();
};
