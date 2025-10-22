#include "input.h"
#include "event.h"
#include <SDL3/SDL_events.h>
#include <spdlog/spdlog.h>

Input::Input() : d{} {
}

Input::~Input() {
	spdlog::info("Input::~Input");
}

void Input::winResize(SDL_WindowEvent *w) {
	d.winW = w->data1;
	d.winH = w->data2;
}

void Input::key(SDL_KeyboardEvent *e) {
	switch (e->key) {
	case SDLK_ESCAPE:
		d.quit = true;
		break;
	case SDLK_UP:
	case SDLK_RIGHT:
		d.speed = 1;
		break;
	case SDLK_DOWN:
	case SDLK_LEFT:
		d.speed = -1;
		break;
	}
}

void Input::gamepadButton(SDL_GamepadButtonEvent *e, bool down) {
	switch (e->button) {
	case SDL_GAMEPAD_BUTTON_START:
		d.quit = true;
		break;
	default:
		std::string s = getSDLGamepadBtnName(e->button);
		spdlog::info("gamepad button {}", s);
		break;
	}
}

void Input::Reset() {
	d = {};
}
