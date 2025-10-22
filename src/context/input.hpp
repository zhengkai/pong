#pragma once

#include "../util/event.hpp"
#include <SDL3/SDL_events.h>

namespace context {
struct Input {

public:
	bool quit = false;
	bool stop = false;
	float x = 0.0f;
	float y = 0.0f;
	int winW = 0;
	int winH = 0;
	int speed = 0;

	void Reset() {
		*this = {};
	};

	void winResize(SDL_WindowEvent *w) {
		winW = w->data1;
		winH = w->data2;
	}

	void key(SDL_KeyboardEvent *e) {
		switch (e->key) {
		case SDLK_ESCAPE:
			quit = true;
			break;
		case SDLK_UP:
		case SDLK_RIGHT:
			speed = 1;
			break;
		case SDLK_DOWN:
		case SDLK_LEFT:
			speed = -1;
			break;
		}
	};
	void gamepadButton(SDL_GamepadButtonEvent *e, bool down) {
		switch (e->button) {
		case SDL_GAMEPAD_BUTTON_START:
			quit = true;
			break;
		default:
			std::string s = util::getSDLGamepadBtnName(e->button);
			spdlog::info("gamepad button {}", s);
			break;
		}
	};
};

}; // namespace context
