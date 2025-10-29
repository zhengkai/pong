#pragma once

#include "../config.hpp"
#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>

inline int parseArg(int argc, char **argv) {
	CLI::App app{"Pong"};

	app.add_flag("-v,--version", config::versionOnly, "Print version only");

	int regionNum;
	app.add_option("-r,--region-num", regionNum, "Region number")
		->default_val(config::regionNumDefault)
		->check(CLI::Range(2, 100));

	int w;
	app.add_option("-W,--grid-w", w, "Grid width")
		->default_val(config::gridWDefault)
		->check(CLI::Range(10, 100));
	int h;
	app.add_option("-H,--grid-h", h, "Grid height")
		->default_val(config::gridHDefault)
		->check(CLI::Range(10, 100));

	app.add_option(
		   "--verbose", config::verbose, "Verbose level (info/debug/trace)")
		->default_val("info")
		->expected(0, 1)
		->check(CLI::IsMember(
			{"info", "debug", "trace", "warn", "err", "critical", "off"}));

	CLI11_PARSE(app, argc, argv);

	config::setGrid(w, h);
	config::setRegion(regionNum);

	return 0;
}
