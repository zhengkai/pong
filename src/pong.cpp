#include "pong.h"
#include "input.h"
#include "sdl.h"
#include "spdlog/spdlog.h"
#include <cstring>

static int sizeW = 10;
static int sizeH = sizeW;

Pong::Pong() {
	t = new Time(4.0f);
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

	loop();
	loop();
	// while (isRunning) {
	// loop();
	// break;
	// }
}

Pong::~Pong() {
	if (t) {
		delete t;
		t = nullptr;
	}
	if (input) {
		delete input;
		input = nullptr;
	}
}

void Pong::loop() {
	t->tick();
}
