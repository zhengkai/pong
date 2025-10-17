#pragma once

#include "box2d/base.h"
#include "build.h"
#include "spdlog/spdlog.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdio>
#include <cstring>

static void doShowVer() {
	spdlog::info("SDL       = {}.{}.{}",
		SDL_MAJOR_VERSION,
		SDL_MINOR_VERSION,
		SDL_MICRO_VERSION);
	spdlog::info("SDL ttf   = {}.{}.{}",
		SDL_TTF_MAJOR_VERSION,
		SDL_TTF_MINOR_VERSION,
		SDL_TTF_MICRO_VERSION);
	spdlog::info("SDL image = {}.{}.{}",
		SDL_IMAGE_MAJOR_VERSION,
		SDL_IMAGE_MINOR_VERSION,
		SDL_IMAGE_MICRO_VERSION);

	b2Version v = b2GetVersion();
	spdlog::info("Box2D     = {}.{}.{}", v.major, v.minor, v.revision);

	spdlog::info("git = {}({}{})",
		GIT_BRANCH,
		GIT_HASH,
		(strcmp(GIT_DIRTY, "yes") == 0) ? "-dirty" : "");
	spdlog::info("build time = {}", BUILD_TIMESTAMP);
}

inline bool showVer(int argc, char *argv[]) {
	for (int i = 1; i < argc; ++i) {
		if (std::strcmp(argv[i], "--version") == 0) {
			doShowVer();
			return true;
		}
	}
	return false;
}
