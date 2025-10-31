#include "sdl.h"
#include "config.hpp"
#include "context/entity.h"
#include "context/window.h"
#include "render/text.h"
#include "util/path.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <memory>
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

	spdlog::info("desktop get size {}x{}", mode->w, mode->h);

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

	rainbow = std::make_unique<Rainbow>(r);

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
		spdlog::trace("text inited");
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

	SDL_SetRenderDrawColor(r, 16, 64, 128, 255);
	SDL_RenderClear(r);

	renderBrick();

	if (d.window->showBall) {
		for (auto &b : d.entity->ballList) {
			renderBall(b);
		}
	}

	rainbow->render(d.entity->ballList, d.window);

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

	auto bl = d.entity->ballList;

	for (auto &b : bl) {
		b->power = 0;
	}
	for (const auto &b : d.entity->brick) {
		bl[b.region]->power++;
	}

	SDL_FRect rect;
	rect.w = w->gridSize;
	rect.h = w->gridSize;

	for (const auto &b : d.entity->brick) {
		rect.x = w->startX + b.x * w->gridSize;
		rect.y = w->startY + b.y * w->gridSize;
		auto ball = bl[b.region];
		auto c = util::HCT(ball->hue, 80, b.tone).ToColor();
		SDL_SetRenderDrawColor(r, c.r, c.g, c.b, 255);
		SDL_RenderFillRect(r, &rect);
	}
}

void sdl::renderBall(std::shared_ptr<context::Ball> b) {

	auto w = d.window;

	SDL_FRect rect;
	rect.x = w->startX + (b->pos.x - config::ballRadius) * w->gridSize;
	rect.y = w->startY + (b->pos.y - config::ballRadius) * w->gridSize;
	rect.w = w->gridSize * config::ballRadius / 0.5f;
	rect.h = rect.w;

	spdlog::trace("ball = {} {} {}", rect.x, rect.y, rect.w);

	auto bc = util::HCT(b->hue, 80, 80).ToColor();

	SDL_SetTextureColorMod(ballTex, bc.r, bc.g, bc.b);

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

	float wf = config::gridWF;
	float hf = config::gridHF + cfgPaddingTop;

	float gs = std::floor(ww / wf < wh / hf ? ww / wf : wh / hf);

	w->gridSize = gs;
	spdlog::info("gridSize = {}, win = {}x{}", gs, winW, winH);

	w->startX = std::floor((ww - (gs * wf)) / 2);
	w->startY = std::floor((wh - (gs * hf)) / 2 + (gs * cfgPaddingTop));
	spdlog::info(
		"grid {}x{}={}, grid pixel: {}x{}, win pixel: {}x{}, start: x={},y={}",
		config::gridW,
		config::gridH,
		config::gridW * config::gridH,
		gs * config::gridW,
		gs * config::gridH,
		w->w,
		w->h,
		w->startX,
		w->startY);
}

sdl::~sdl() {
	spdlog::trace("sdl::~sdl");
	if (text) {
		delete text;
		text = nullptr;
	}

	if (ballTex) {
		SDL_DestroyTexture(ballTex);
		ballTex = nullptr;
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
