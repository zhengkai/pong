#include "pong.h"
#include "sys/debug.h"
#include "sys/ver.h"

int main(int argc, char *argv[]) {

	if (showVer(argc, argv)) {
		return 0;
	}

	if (runPong(argc, argv)) {
		return 0;
	}

	checkDebug(argc, argv);

	// config = new Config();

	Pong pong;
	pong.run();

	// delete config;
	// config = nullptr;

	return 0;
}
