#include "sdl.h"
#include "SDL3/SDL_events.h"
#include "config.hpp"
#include "input.h"
#include "render/grid.h"
#include "render/layout.hpp"
#include "render/text.h"
#include "spdlog/spdlog.h"
#include <SDL3/SDL.h>
#include <random>
#include <vector>

static float winW = static_cast<float>(cfgWinW);
static float winH = static_cast<float>(cfgWinH);

static SDL_AppResult SDL_Fail() {
	SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
	return SDL_APP_FAILURE;
}

sdl::sdl(sdlDep dep, Input *in)
	: input(in), window(nullptr), r(nullptr), d(std::move(dep)) {
	spdlog::info("ptr = {}", static_cast<void *>(input));

	for (const auto &b : d.entity->brick) {
		spdlog::trace("brick {} {} {} {}", b.id, b.x, b.y, b.region);
	}
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

	SDL_Surface *surf = IMG_Load("/www/pong/static/circle.webp");
	if (!surf) {
		spdlog::error("Failed to load ball image");
		return false;
	}
	ballTex = SDL_CreateTextureFromSurface(r, surf);
	SDL_DestroySurface(surf);

	if (!ballTex) {
		spdlog::error("ball SDL_CreateTextureFromSurface failed");
		return false;
	}

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

void sdl::renderBrick() {

	Layout &layout = Layout::instance();

	SDL_FRect rect;
	rect.w = layout.gridSize;
	rect.h = layout.gridSize;

	for (const auto &b : d.entity->brick) {
		rect.x = layout.startX + static_cast<float>(b.x) * layout.gridSize;
		rect.y = layout.startY + static_cast<float>(b.y) * layout.gridSize;
		switch (b.region) {
		case 1:
			SDL_SetRenderDrawColor(r, 255, 0, 0, 180);
			break;
		case 2:
			SDL_SetRenderDrawColor(r, 255, 127, 0, 180);
			break;
		case 3:
			SDL_SetRenderDrawColor(r, 255, 255, 0, 180);
			break;
		case 4:
			SDL_SetRenderDrawColor(r, 0, 255, 0, 180);
			break;
		case 5:
			SDL_SetRenderDrawColor(r, 0, 206, 209, 180);
			break;
		case 6:
			SDL_SetRenderDrawColor(r, 0, 0, 255, 180);
			break;
		case 7:
			SDL_SetRenderDrawColor(r, 75, 0, 130, 180);
			break;
		case 8:
			SDL_SetRenderDrawColor(r, 148, 0, 211, 180);
			break;
		default:
			SDL_SetRenderDrawColor(r, 255, 20, 147, 180);
			break;
		}
		SDL_RenderFillRect(r, &rect);
	}
}

void sdl::renderBall(std::shared_ptr<context::Ball> b) {
	Layout &layout = Layout::instance();

	SDL_FRect rect;
	rect.x = layout.startX + (b->pos.x - 0.5) * layout.gridSize;
	rect.y = layout.startY + (b->pos.y - 0.5) * layout.gridSize;
	rect.w = layout.gridSize;
	rect.h = layout.gridSize;

	spdlog::trace("ball = {} {} {}", rect.x, rect.y, rect.w);
	SDL_RenderTexture(r, ballTex, nullptr, &rect);
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
		// li[i] = dist(gen);
		li[i] = false;
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
