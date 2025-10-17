#include "pong.h"
#include "config.hpp"
#include "event.h"
#include "input.h"
#include "region.hpp"
#include "sdl.h"
#include "spdlog/spdlog.h"
#include "util/ball.hpp"
#include <SDL3/SDL_events.h>
#include <cstring>
#include <thread>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Pong::Pong() : stop(false), input(new Input()), t(new Time()) {
}

Pong::~Pong() {
	stop = true;
#ifndef __EMSCRIPTEN__
	if (bgThread.joinable()) {
		bgThread.join();
	}
#endif
	if (t) {
		delete t;
		t = nullptr;
	}
	if (s) {
		delete s;
		s = nullptr;
	}
	if (input) {
		delete input;
		input = nullptr;
	}
}

bool Pong::init() {

	auto e = std::make_shared<context::Entity>();

	d = {
		.entity = e,
	};

	int id = 0;
	// e->brick.reserve(cfgGridW * cfgGridH);

	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<int> dist(0, cfgRegionNum - 1);

	for (int x = 0; x < cfgGridW; x++) {
		for (int y = 0; y < cfgGridH; y++) {
			e->brick.push_back({
				.id = id,
				.x = x,
				.y = y,
				.region = dist(g),
			});
			id++;
		}
	}

	context::BallList =
		util::generateBall(cfgGridWF, cfgGridHF, cfgRegionNum, 3);
	for (auto &b : context::BallList) {
		region.push_back(std::make_unique<Region>(e, b));
	}

	s = new sdl(
		{
			.entity = d.entity,
		},
		input);

	spdlog::info("pong start");

	if (!s->init()) {
		spdlog::info("sdl init failed");
		return false;
	}
	spdlog::info("sdl init done");

#ifdef __EMSCRIPTEN__
	startBg();
#else
	bgThread = std::thread(&Pong::sdlBg, this);
	bgThread.detach();
#endif
	return true;
}

void Pong::loop() {

	serial++;

#ifndef __EMSCRIPTEN__
	t->tick();
#endif

	if (input->stop) {
		stop = true;
		return;
	}

	std::random_device rd;
	std::mt19937 g(rd());

	for (int i = 0; i < cfgPhyLoop; i++) {
		std::shuffle(region.begin(), region.end(), g);
		for (auto &r : region) {
			r->update();
		}
	}

	s->renderStart();
	s->renderBrick();

	for (auto &b : context::BallList) {
		s->renderBall(b);
	}

	// s->renderBall(d.entity->ballA);
	// s->renderBallB();
	s->renderGrid();
	s->counter(serial);
	SDL_RenderPresent(s->r);
}

void Pong::sdlBg() {
	spdlog::info("sdl bg start");

	SDL_Event event;
	while (!stop) {
		SDL_WaitEventTimeout(&event, 0);
		SDLEventLog(event.type);
		if (event.type == SDL_EVENT_QUIT) {
			break;
		}
		// spdlog::info("sdlBg handleInput");
		s->handleInput(&event);
	}
	stop = true;
}

void Pong::sdlBgStep() {
#ifdef __EMSCRIPTEN__
	if (stop) {
		return;
	}

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		SDLEventLog(event.type);
		if (event.type == SDL_EVENT_QUIT) {
			stop = true;
			break;
		}
		s->handleInput(&event);
	}

	// 递归调用自己，在浏览器下一帧继续执行
	emscripten_async_call(
		[](void *arg) { static_cast<Pong *>(arg)->sdlBgStep(); }, this, 0);
#endif
}

void Pong::startBg() {
	stop = false;
#ifdef __EMSCRIPTEN__
	sdlBgStep(); // 启动异步循环
#endif
}

void Pong::stopBg() {
	stop = true;
}
