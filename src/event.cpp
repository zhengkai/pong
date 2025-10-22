#include "event.h"
#include "context/input.hpp"
#include <SDL3/SDL_events.h>
#include <spdlog/spdlog.h>

static void handleInput(SDL_Event *e, std::shared_ptr<context::Input> input) {

	SDL_Gamepad *gamepad;

	switch (e->type) {
	case SDL_EVENT_KEY_DOWN:
		input->key(&e->key);
		break;
	case SDL_EVENT_WINDOW_RESIZED: {
		input->winResize(&e->window);
		break;
	}

	default:
		break;
	}
}

void loopEvent(std::shared_ptr<context::Input> input) {
	SDL_Event e;
	input->Reset();
	while (SDL_PollEvent(&e)) {
		util::SDLEventLog(e.type);
		if (e.type == SDL_EVENT_QUIT) {
			input->quit = true;
			break;
		}
		handleInput(&e, input);
	}
}
