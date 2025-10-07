#include "sdl.h"
#include "SDL3/SDL_events.h"
#include "input.h"
#include <SDL3/SDL.h>

static int winW = 1000;
static int winH = winW;

static SDL_AppResult SDL_Fail() {
	SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
	return SDL_APP_FAILURE;
}

sdl::sdl(Input *input) : input(input), window(nullptr), r(nullptr) {
	input = input;
}

bool sdl::init() {

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Fail();
		return false;
	}

	window = SDL_CreateWindow("Pong Test",
		winW,
		winH,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
	if (!window) {
		SDL_Fail();
		return false;
	}

	SDL_HideCursor();
	SDL_SetWindowMouseGrab(window, true);

	SDL_Renderer *r = SDL_CreateRenderer(window, NULL);
	if (!r) {
		SDL_Fail();
		return false;
	}

	SDL_SetRenderDrawColor(r, 64, 64, 64, 255);
	SDL_RenderClear(r);

	SDL_RenderPresent(r);

	return true;
}

void sdl::handleInput(SDL_Event *e) {

	SDL_Gamepad *gamepad;

	switch (e->type) {
	case SDL_EVENT_KEY_DOWN:
		input->key(&e->key);
		break;
	default:
		break;
	}
}

sdl::~sdl() {
	input = nullptr;
	if (r) {
		SDL_DestroyRenderer(r);
		r = nullptr;
	}
	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}
	SDL_Quit();
}
