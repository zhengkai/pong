#include "sdl.h"
#include "SDL3/SDL_events.h"
#include "config.hpp"
#include "input.h"
#include "render/text.h"
#include "render/grid.h"
#include "spdlog/spdlog.h"
#include <SDL3/SDL.h>
#include <vector>
#include <random>

static float winW = static_cast<float>(cfgWinW);
static float winH = static_cast<float>(cfgWinH);

static SDL_AppResult SDL_Fail() {
	SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
	return SDL_APP_FAILURE;
}

sdl::sdl(Input *in) : input(in), window(nullptr), r(nullptr) {
	spdlog::info("ptr = {}", static_cast<void *>(input));
}

bool sdl::init() {

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Fail();
		return false;
	}

	window = SDL_CreateWindow("Pong Test",
		cfgWinW,
		cfgWinH,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
	if (!window) {
		SDL_Fail();
		return false;
	}

	SDL_HideCursor();
	// SDL_SetWindowMouseGrab(window, true);

	r = SDL_CreateRenderer(window, NULL);
	if (!r) {
		SDL_Fail();
		return false;
	}

	SDL_SetRenderDrawColor(r, 64, 64, 64, 255);
	SDL_RenderClear(r);

	SDL_RenderPresent(r);

	grid = new Grid(r);

	text = new Text();
	if (text->init(r)) {
		spdlog::info("text inited");
	} else {
		spdlog::error("Failed to init text");
		return false;
	}

	return true;
}

void sdl::counter(int i) {
	text->rMono32(std::to_string(i), winW - 16, 16, Text::Align::RIGHT);
}

void sdl::renderStart() {
	SDL_SetRenderDrawColor(r, 16, 64, 128, 64);
	SDL_RenderClear(r);
}
void sdl::renderEnd() {
	// SDL_RenderPresent(r);
}

void sdl::renderGrid() {

    std::vector<bool> li(cfgGridW * cfgGridH);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dist(0.5);

	for (size_t i = 0; i < li.size(); ++i) {
		li[i] = dist(gen);
	}

	grid->draw(li);
}

void sdl::handleInput(SDL_Event *e) {

	SDL_Gamepad *gamepad;

	switch (e->type) {
	case SDL_EVENT_KEY_DOWN:
		spdlog::info("ptr = {}", static_cast<void *>(input));
		input->key(&e->key);
		break;
	default:
		break;
	}
	// spdlog::info("sdl::handleInput done");
}

sdl::~sdl() {
	spdlog::info("sdl::~sdl");
	input = nullptr;
	if (text) {
		delete text;
		text = nullptr;
	}
	if (grid) {
		delete grid;
		grid = nullptr;
	}
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
