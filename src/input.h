#pragma once

#include <SDL3/SDL_events.h>

class Input {

public:
	bool stop;
	float x;
	float y;

public:
	Input();
	~Input();
	void Reset();
	void key(SDL_KeyboardEvent *e);
	void gamepadButton(SDL_GamepadButtonEvent *e, bool down);
};
