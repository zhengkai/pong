#include "input.h"
#include "event.h"
#include <SDL3/SDL_events.h>
#include <spdlog/spdlog.h>

Input::Input() : stop(false), x(0), y(0) {
}

Input::~Input() {
	spdlog::info("Input::~Input");
}

void Input::key(SDL_KeyboardEvent *e) {
	if (e->key == SDLK_ESCAPE) {
		spdlog::info("esc key");
		stop = true;
		spdlog::info("esc key done");
		return;
	}
}

void Input::gamepadButton(SDL_GamepadButtonEvent *e, bool down) {
	switch (e->button) {
	case SDL_GAMEPAD_BUTTON_START:
		Input::stop = true;
		break;
	default:
		std::string s = getSDLGamepadBtnName(e->button);
		spdlog::info("gamepad button {}", s);
		break;
	}
}

void Input::Reset() {
	stop = false;
	x = 0;
	y = 0;
}
