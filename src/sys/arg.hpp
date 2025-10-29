#pragma once

#include "../config.hpp"
#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>

inline int parseArg(int argc, char **argv) {
	CLI::App app{"Pong"};

	app.add_flag("-v,--version", config::versionOnly, "Print version only");

	app.add_option("-r,--region-num", config::regionNum, "Region number")
		->default_val(config::regionNumDefault);

	app.add_option(
		   "--verbose", config::verbose, "Verbose level (info/debug/trace)")
		->default_val("info")
		->check(CLI::IsMember(
			{"info", "debug", "trace", "warn", "err", "critical", "off"}));

	CLI11_PARSE(app, argc, argv);
	return 0;
}
