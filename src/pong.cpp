#include "pong.h"
#include "event.h"
#include "input.h"
#include "sdl.h"
#include "spdlog/spdlog.h"
#include <SDL3/SDL_events.h>
#include <cstring>
#include <thread>

static int sizeW = 10;
static int sizeH = sizeW;

Pong::Pong() {
	t = new Time(10.0f);
	isRunning = true;
	input = new Input();
	spdlog::info("pong start");
}

void Pong::run() {
	sdl s(input);
	if (!s.init()) {
		spdlog::info("sdl init failed");
		return;
	}
	spdlog::info("sdl init done");

	std::thread t(&Pong::sdlBg, this);
	t.detach();

	for (int i = 0; i < 10; i++) {
		loop();
	}
	isRunning = false;
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

void Pong::loop() {
	t->tick();
}

void Pong::sdlBg() {

	spdlog::info("sdl bg start");

	int timeout_ms = 10;

	SDL_Event event;
	while (isRunning) {
		SDL_WaitEventTimeout(&event, timeout_ms);
		SDLEventLog(event.type);
		if (event.type == SDL_EVENT_QUIT) {
			break;
		}
		s->handleInput(&event);
	}
	isRunning = false;
}
