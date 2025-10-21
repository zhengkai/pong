#include "sdl.h"
#include "config.hpp"
#include "context/entity.h"
#include "input.h"
#include "render/grid.h"
#include "render/text.h"
#include "util/path.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <spdlog/spdlog.h>

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
		SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE |
			SDL_WINDOW_HIGH_PIXEL_DENSITY);
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
	auto img = util::file("circle.webp");
	spdlog::info("img {}", img);

	SDL_Surface *surf = IMG_Load(img.c_str());
	if (!surf) {
		spdlog::error("Failed to load ball image");
		SDL_Fail();
		return false;
	}
	ballTex = SDL_CreateTextureFromSurface(r, surf);
	SDL_DestroySurface(surf);

	if (!ballTex) {
		spdlog::error("ball SDL_CreateTextureFromSurface failed");
		SDL_Fail();
		return false;
	}

	// int drawableWidth, drawableHeight;
	// SDL_GetCurrentRenderOutputSize(r, &drawableWidth, &drawableHeight);
	// spdlog::error("output size {} {}", drawableWidth, drawableHeight);

	calcGrid(cfgWinW, cfgWinH, d.window);

	text = new Text();
	if (text->init(r)) {
		spdlog::info("text inited");
	} else {
		spdlog::error("Failed to init text");
		return false;
	}

	return true;
}

void sdl::renderCcounter() {

	std::string counter = std::to_string(d.window->counter);
	text->rMono32(counter, winW - 16, 16, Text::Align::RIGHT);
}

void sdl::render() {
	renderResize();

	renderStart();
	renderBrick();

	for (auto &b : context::BallList) {
		renderBall(b);
	}

	renderCcounter();
	SDL_RenderPresent(r);
}

void sdl::renderResize() {
	if (input->winW == 0 || input->winH == 0) {
		return;
	}
	calcGrid(input->winW, input->winH, d.window);
}

void sdl::renderBrick() {

	auto w = d.window;

	SDL_FRect rect;
	rect.w = w->gridSize;
	rect.h = w->gridSize;

	for (const auto &b : d.entity->brick) {
		rect.x = w->startX + static_cast<float>(b.x) * w->gridSize;
		rect.y = w->startY + static_cast<float>(b.y) * w->gridSize;
		switch (b.region) {
		case 1:
			SDL_SetRenderDrawColor(r, 255, 0, 0, 180);
			break;
		case 2:
			SDL_SetRenderDrawColor(r, 0, 0, 255, 180);
			break;
		case 3:
			SDL_SetRenderDrawColor(r, 75, 0, 130, 180);
			break;
		case 4:
			SDL_SetRenderDrawColor(r, 148, 0, 211, 180);
			break;
		case 5:
			SDL_SetRenderDrawColor(r, 0, 206, 209, 180);
			break;
		case 6:
			SDL_SetRenderDrawColor(r, 255, 255, 0, 180);
			break;
		case 7:
			SDL_SetRenderDrawColor(r, 255, 20, 147, 180);
			break;
		case 8:
			SDL_SetRenderDrawColor(r, 255, 127, 0, 180);
			break;
		default:
			SDL_SetRenderDrawColor(r, 0, 255, 0, 180);
			break;
		}
		SDL_RenderFillRect(r, &rect);
	}
}

void sdl::renderBall(std::shared_ptr<context::Ball> b) {

	auto w = d.window;

	SDL_FRect rect;
	rect.x = w->startX + (b->pos.x - 0.5) * w->gridSize;
	rect.y = w->startY + (b->pos.y - 0.5) * w->gridSize;
	rect.w = w->gridSize;
	rect.h = w->gridSize;

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

void sdl::handleInput(SDL_Event *e) {

	SDL_Gamepad *gamepad;

	switch (e->type) {
	case SDL_EVENT_KEY_DOWN:
		spdlog::info("ptr = {}", static_cast<void *>(input));
		input->key(&e->key);
		break;
	case SDL_EVENT_WINDOW_RESIZED: {
		input->winW = e->window.data1;
		input->winH = e->window.data2;
		break;
	}

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
