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

Pong::Pong() : stop(false), t(new Time()) {
}

Pong::~Pong() {
	stop = true;
	if (t) {
		delete t;
		t = nullptr;
	}
	if (s) {
		delete s;
		s = nullptr;
	}
	if (g) {
		delete g;
		g = nullptr;
	}
}

bool Pong::init() {

	auto e = std::make_shared<context::Entity>();
	auto w = std::make_shared<context::Window>();
	e->ballList = util::generateBall(cfgGridWF, cfgGridHF, cfgRegionNum, 3);

	d = {
		.entity = e,
		.window = w,
	};

	std::uniform_int_distribution<int> dist(0, cfgRegionNum - 1);

	int id = 0;
	for (int x = 0; x < cfgGridW; x++) {
		for (int y = 0; y < cfgGridH; y++) {
			e->brick.push_back({
				.id = id,
				.x = static_cast<float>(x),
				.y = static_cast<float>(y),
				.region = dist(util::rng()),
				.tone = 50.0,
			});
			id++;
		}
	}

	for (auto &b : e->ballList) {
		region.push_back(std::make_unique<Region>(e, b));
	}

	g = new Game({
		.entity = e,
		.window = w,
	});

	s = new sdl({
		.entity = e,
		.window = w,
	});

	spdlog::info("pong start");

	if (!s->init()) {
		spdlog::info("sdl init failed");
		return false;
	}
	spdlog::info("sdl init done");

	return true;
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
