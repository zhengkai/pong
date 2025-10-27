#include "sdl.h"
#include "config.hpp"
#include "context/entity.h"
#include "render/text.h"
#include "util/path.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <spdlog/spdlog.h>

static SDL_AppResult SDL_Fail() {
	SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
	return SDL_APP_FAILURE;
}

sdl::sdl(sdlDep dep) : w(nullptr), r(nullptr), d(std::move(dep)) {
	for (const auto &b : d.entity->brick) {
		spdlog::trace("brick {} {:.0f} {:.0f} {}", b.id, b.x, b.y, b.region);
	}
}

void sdl::initWinSize() {

	SDL_DisplayID display = SDL_GetPrimaryDisplay();
	if (!display) {
		return;
	}
	auto mode = SDL_GetCurrentDisplayMode(display);
	if (!mode) {
		return;
	}

	spdlog::info("win get size {} {}", mode->w, mode->h);

	d.window->w = std::min(cfgWinW, mode->w);
	d.window->h = std::min(cfgWinH, mode->h);
}

bool sdl::init() {

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Fail();
		return false;
	}

	initWinSize();

	SDL_PropertiesID props = SDL_CreateProperties();

	SDL_SetStringProperty(
		props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "Pong Test");
	SDL_SetNumberProperty(
		props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, d.window->w);
	SDL_SetNumberProperty(
		props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, d.window->h);
	SDL_SetNumberProperty(props,
		SDL_PROP_WINDOW_CREATE_FLAGS_NUMBER,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

#ifdef __EMSCRIPTEN__
	// SDL_SetStringProperty(
	// props, SDL_PROP_WINDOW_CREATE_EMSCRIPTEN_CANVAS_ID_STRING, "#pong");
#endif

	w = SDL_CreateWindowWithProperties(props);

	SDL_DestroyProperties(props);

	spdlog::info("win create size {} {}", d.window->w, d.window->h);
	if (!w) {
		SDL_Fail();
		return false;
	}

	// SDL_HideCursor();
	// SDL_SetWindowMouseGrab(window, true);

	r = SDL_CreateRenderer(w, NULL);
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

	calcGrid(cfgWinW, cfgWinH);

	text = new Text();
	if (text->init(r)) {
		spdlog::info("text inited");
	} else {
		spdlog::error("Failed to init text");
		return false;
	}

	return true;
}

void sdl::renderCounter() {

	std::string counter = std::to_string(d.window->serial);
	text->rMono32(counter, d.window->w - 16, 16, Text::Align::RIGHT);
}

void sdl::render() {
	renderResize();

	SDL_SetRenderDrawColor(r, 16, 64, 128, 64);
	SDL_RenderClear(r);

	renderBrick();

	if (d.window->showBall) {
		for (auto &b : context::BallList) {
			renderBall(b);
		}
	}

	renderCounter();
	renderControlMsg();

	SDL_RenderPresent(r);
}

void sdl::renderResize() {
	auto wr = d.window->winResize;
	if (wr == nullptr) {
		return;
	}
	d.window->winResize = nullptr;
	calcGrid(wr->w, wr->h);
	delete wr;
}

void sdl::renderBrick() {

	auto w = d.window;

	SDL_FRect rect;
	rect.w = w->gridSize;
	rect.h = w->gridSize;

	for (const auto &b : d.entity->brick) {
		rect.x = w->startX + b.x * w->gridSize;
		rect.y = w->startY + b.y * w->gridSize;
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
	rect.x = w->startX + (b->pos.x - cfgBallRadius) * w->gridSize;
	rect.y = w->startY + (b->pos.y - cfgBallRadius) * w->gridSize;
	rect.w = w->gridSize * cfgBallRadius / 0.5f;
	rect.h = rect.w;

	spdlog::trace("ball = {} {} {}", rect.x, rect.y, rect.w);
	SDL_RenderTexture(r, ballTex, nullptr, &rect);
}

void sdl::renderControlMsg() {
	context::ControlMsg *c = d.window->controlMsg;
	if (c == nullptr) {
		return;
	}
	if (c->expireSerial < d.window->serial) {
		delete c;
		d.window->controlMsg = nullptr;
		return;
	}
	text->rMono96(
		c->msg, d.window->w / 2, d.window->h - 192, Text::Align::CENTER);
}

void sdl::calcGrid(int winW, int winH) {

	float scale = SDL_GetWindowDisplayScale(w);
	spdlog::info("start sdl::calcGrid, window display scale {:.1f}", scale);

	auto w = d.window;

	float ww = std::floor(static_cast<float>(winW) * scale);
	float wh = std::floor(static_cast<float>(winH) * scale);
	w->w = static_cast<int>(ww);
	w->h = static_cast<int>(wh);

	float gs = std::floor(
		ww / cfgGridWF < wh / cfgGridHF ? ww / cfgGridWF : wh / cfgGridHF);

	w->gridSize = gs;
	spdlog::info("gridSize = {}, win = {}x{}", gs, winW, winH);

	w->startX = std::floor((ww - (gs * cfgGridW)) / 2);
	w->startY = std::floor((wh - (gs * cfgGridH)) / 2);
	spdlog::info("w {}*{}={}", gs, cfgGridW, gs * cfgGridW);
	spdlog::info("h {}*{}={}", gs, cfgGridH, gs * cfgGridH);
	spdlog::info("startX = {}, startY = {}", w->startX, w->startY);

	spdlog::info("w {}*{}={}", gs, cfgGridW, gs * cfgGridW);
}

sdl::~sdl() {
	spdlog::info("sdl::~sdl");
	if (text) {
		delete text;
		text = nullptr;
	}
	if (r) {
		SDL_DestroyRenderer(r);
		r = nullptr;
	}
	if (w) {
		SDL_DestroyWindow(w);
		w = nullptr;
	}
	SDL_Quit();
}
