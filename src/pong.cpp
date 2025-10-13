#include "pong.h"
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

	d = {
		.entity = std::make_shared<context::Entity>(),
	};

	p = new Physics({
		.entity = d.entity,
	});

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
	if (p) {
		delete p;
		p = nullptr;
	}
}

void Pong::loop(int cnt) {
	t->tick();
	if (input->stop) {
		stop = true;
		return;
	}

	p->update();

	s->renderStart();
	s->renderBall();
	s->renderBallB();
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
