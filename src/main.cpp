#include "pong.h"
#include "sys/debug.hpp"
#include "sys/ver.hpp"
#include "util/hct.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>

static Pong *thePong = nullptr;

void wasmLoop() {
	if (thePong->stop) {
		emscripten_cancel_main_loop();
		return;
	}
	thePong->loop();
}
#endif

int main(int argc, char *argv[]) {

	std::srand(std::time(nullptr));

	if (showVer(argc, argv)) {
		return 0;
	}

	checkDebug(argc, argv);

	Pong p;
	if (!p.init()) {
		return 1;
	}

#ifdef __EMSCRIPTEN__
	thePong = &p;
	emscripten_set_main_loop(wasmLoop, 0, true);
#else
	while (!p.stop) {
		p.loop();
	}
#endif

	return 0;
}
