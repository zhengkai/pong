#include "pong.h"
#include "sys/arg.hpp"
#include "sys/spdlog.hpp"
#include "sys/ver.hpp"
#include "util/path.hpp"
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

	util::InitDir();

	parseArg(argc, argv);

	spdlogInit();

	doShowVer();
	if (config::versionOnly) {
		return 0;
	}

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
