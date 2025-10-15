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

static int sizeW = 10;
static int sizeH = sizeW;

Pong::Pong() : stop(false), input(new Input()), t(new Time()) {

	init();

	s = new sdl(
		{
			.entity = d.entity,
		},
		input);

	spdlog::info("pong start");
}

void Pong::run() {
	if (!s->init()) {
		spdlog::info("sdl init failed");
		return;
	}
	spdlog::info("sdl init done");

	std::thread t(&Pong::sdlBg, this);
	t.detach();

	int i = 0;
	while (!stop) {
		i++;
		loop(i);
	}
	stop = true;
	spdlog::info("Pong::run stop");
	// std::this_thread::sleep_for(std::chrono::seconds(1));
}

Pong::~Pong() {
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

void Pong::init() {

	auto e = std::make_shared<context::Entity>();

	d = {
		.entity = e,
	};

	int id = 0;
	// e->brick.reserve(cfgGridW * cfgGridH);
	for (int x = 0; x < cfgGridW; x++) {
		for (int y = 0; y < cfgGridH; y++) {
			e->brick.push_back({
				.id = id,
				.x = x,
				.y = y,
				.region = (x < (cfgGridW / 2)) ? 2 : 1,
			});
			id++;
		}
	}

	context::BallList = util::generateBall(cfgGridWF, cfgGridHF, 1, 3);
	// region.reserve(context::BallList.size());
	for (auto &b : context::BallList) {
		region.push_back(std::make_unique<Region>(e, b));
	}
}

void Pong::loop(int cnt) {
	t->tick();
	if (input->stop) {
		stop = true;
		return;
	}

	for (auto &r : region) {
		r->update();
	}

	s->renderStart();
	s->renderBrick();

	for (auto &b : context::BallList) {
		s->renderBall(b);
	}

	// s->renderBall(d.entity->ballA);
	// s->renderBallB();
	s->renderGrid();
	s->counter(cnt);
	SDL_RenderPresent(s->r);
}

void Pong::sdlBg() {
	spdlog::info("sdl bg start");

	int timeout_ms = 100;

	SDL_Event event;
	while (!stop) {
		SDL_WaitEventTimeout(&event, timeout_ms);
		SDLEventLog(event.type);
		if (event.type == SDL_EVENT_QUIT) {
			break;
		}
		// spdlog::info("sdlBg handleInput");
		s->handleInput(&event);
	}
	stop = true;
}
