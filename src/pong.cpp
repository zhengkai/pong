#include "pong.h"
#include "config.hpp"
#include "context/entity.h"
#include "context/window.h"
#include "game.h"
#include "region.hpp"
#include "sdl.h"
#include "util/ball.hpp"
#include "util/rand.hpp"
#include <SDL3/SDL_events.h>
#include <algorithm>
#include <spdlog/spdlog.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Pong::Pong() : stop(false), t(std::make_unique<Time>()) {
}

Pong::~Pong() {
	stop = true;
}

bool Pong::init() {

	auto e = std::make_shared<context::Entity>();
	auto w = std::make_shared<context::Window>();
	e->ballList = util::generateBall(
		config::gridWF, config::gridHF, config::regionNum, 3);
	e->brick = genBrick();

	d = {
		.entity = e,
		.window = w,
	};

	for (auto &b : e->ballList) {
		region.push_back(std::make_unique<Region>(e, b));
	}

	g = std::make_unique<Game>(GameDep{
		.entity = e,
		.window = w,
	});

	s = std::make_unique<sdl>(sdlDep{
		.entity = e,
		.window = w,
	});

	spdlog::info("pong start");

	if (!s->init()) {
		spdlog::error("sdl init failed");
		return false;
	}
	spdlog::trace("sdl init done");

	return true;
}

std::vector<context::Brick> Pong::genBrick() {

	std::vector<context::Brick> li;

	std::uniform_int_distribution<int> dist(0, config::regionNum - 1);

	int id = 0;
	for (int x = 0; x < config::gridW; x++) {
		for (int y = 0; y < config::gridH; y++) {
			li.push_back({
				.id = id,
				.x = static_cast<float>(x),
				.y = static_cast<float>(y),
				.region = dist(util::rng()),
				.tone = 50.0,
			});
			id++;
		}
	}

	return li;
}

void Pong::loop() {

	d.window->serial++;

#ifndef __EMSCRIPTEN__
	t->tick();
#endif

	g->loopEvent();

	if (!g->parse()) {
		stop = true;
		return;
	}

	for (int i = 0; i < cfgPhyLoop; i++) {
		std::ranges::shuffle(region.begin(), region.end(), util::rng());
		for (auto &r : region) {
			r->update();
		}
	}

	s->render();
}
