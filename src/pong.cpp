#include "pong.h"
#include "input.h"
#include "sdl.h"
#include "spdlog/spdlog.h"
#include <cstring>

static int sizeW = 10;
static int sizeH = sizeW;

bool runPong(int argc, char *argv[]) {
	for (int i = 1; i < argc; ++i) {
		if (std::strcmp(argv[i], "--pong") == 0) {
			Pong pong;
			pong.run();
			return true;
		}
	}
	return false;
}

Pong::Pong() {
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
}

Pong::~Pong() {
	if (input) {
		delete input;
		input = nullptr;
	}
}
