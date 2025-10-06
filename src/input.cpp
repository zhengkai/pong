#include "input.h"
#include "event.h"
#include "spdlog/spdlog.h"
#include <SDL3/SDL_events.h>

Input::Input() {
	Input::stop = false;
}

Input::~Input() {
}

void Input::key(SDL_KeyboardEvent *e) {
	if (e->key == SDLK_ESCAPE) {
		Input::stop = true;
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

Input *Input::Clone() const {
	Input *n = new Input();
	n->x = x;
	n->y = y;
	return n;
}
