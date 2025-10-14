#include "pong.h"
#include "config.hpp"
#include "event.h"
#include "input.h"
#include "physics/physics.h"
#include "sdl.h"
#include "spdlog/spdlog.h"
#include <SDL3/SDL_events.h>
#include <cstring>
#include <thread>

static int sizeW = 10;
static int sizeH = sizeW;

Pong::Pong() : stop(false), input(new Input()), t(new Time()) {

	init();

	pA = new Physics(
		{
			.entity = d.entity,
		},
		1);

	pB = new Physics(
		{
			.entity = d.entity,
		},
		2);

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
	if (pA) {
		delete pA;
		pA = nullptr;
	}
	if (pB) {
		delete pB;
		pB = nullptr;
	}
}

void Pong::init() {

	d = {
		.entity = std::make_shared<context::Entity>(),
	};

	int id = 0;
	for (int x = 0; x < cfgGridW; x++) {
		for (int y = 0; y < cfgGridH; y++) {
			d.entity->brick.push_back({
				.id = id,
				.x = x,
				.y = y,
				.region = (x < (cfgGridW / 2)) ? 2 : 1,
			});
			id++;
		}
	}
}

void Pong::loop(int cnt) {
	t->tick();
	if (input->stop) {
		stop = true;
		return;
	}

	pA->update();
	pB->update();

	s->renderStart();
	s->renderBrick();

	s->renderBall(d.entity->ballA);
	s->renderBall(d.entity->ballB);
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
