#include "debug.h"
#include <cstring>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

bool checkDebug(int argc, char *argv[]) {

	auto console = spdlog::stdout_color_mt("console");

	// 默认日志级别 info
	spdlog::level::level_enum log_level = spdlog::level::info;

	bool verbose = false;

	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg.rfind("--verbose", 0) == 0) {
			verbose = true;
			log_level = spdlog::level::debug; // debug 默认
			if (arg == "--verbose=trace") {
				log_level = spdlog::level::trace;
			}
			break;
		}
	}

	spdlog::set_level(log_level);
	spdlog::set_pattern("%m-%d %H:%M:%S.%e %^[%l]%$ %v");

	return verbose;
}
