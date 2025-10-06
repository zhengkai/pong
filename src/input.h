#pragma once

#include <SDL3/SDL_events.h>

class Input {

public:
	bool stop;

public:
	Input();
	~Input();
	float x;
	float y;
	Input *Clone() const;
	void key(SDL_KeyboardEvent *e);
	void gamepadButton(SDL_GamepadButtonEvent *e, bool down);
};
