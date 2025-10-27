#pragma once

#ifndef __EMSCRIPTEN__
#include <spdlog/async.h>
#include <spdlog/async_logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#endif

inline bool checkDebug(int argc, char *argv[]) {

#ifndef __EMSCRIPTEN__
	spdlog::init_thread_pool(8192, 1);

	auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

	spdlog::async_logger logger("async_stdout",
		{stdout_sink},
		spdlog::thread_pool(),
		spdlog::async_overflow_policy::block);

	spdlog::register_logger(std::make_shared<spdlog::async_logger>(logger));

	spdlog::stdout_color_mt("console");
#endif

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
