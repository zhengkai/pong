#pragma once

#include <cstring>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/async.h"
#include "spdlog/async_logger.h"

bool checkDebug(int argc, char *argv[]) {


    spdlog::init_thread_pool(8192, 1);

    // stdout sink
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    // 创建异步 logger
    spdlog::async_logger logger(
        "async_stdout",
        {stdout_sink},
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::block  // 队列满时阻塞
    );

    spdlog::register_logger(std::make_shared<spdlog::async_logger>(logger));

	spdlog::stdout_color_mt("console");

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
