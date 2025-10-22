#pragma once

#include <SDL3/SDL_events.h>

struct InputData {
	bool quit;
	bool stop;
	float x;
	float y;
	int winW;
	int winH;
	int speed;
};

class Input {

public:
	InputData d;

public:
	Input();
	~Input();
	void Reset();
	void winResize(SDL_WindowEvent *w);
	void key(SDL_KeyboardEvent *e);
	void gamepadButton(SDL_GamepadButtonEvent *e, bool down);
};
