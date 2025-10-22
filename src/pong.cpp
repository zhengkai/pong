#include "pong.h"
#include "config.hpp"
#include "context/entity.h"
#include "context/input.hpp"
#include "context/window.h"
#include "event.h"
#include "game.h"
#include "region.hpp"
#include "sdl.h"
#include "util/ball.hpp"
#include <SDL3/SDL_events.h>
#include <algorithm>
#include <cstring>
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
	auto in = std::make_shared<context::Input>();

	d = {
		.entity = e,
		.window = w,
		.input = in,
	};

	std::random_device rd;
	std::mt19937 grd(rd());
	std::uniform_int_distribution<int> dist(0, cfgRegionNum - 1);

	int id = 0;
	for (int x = 0; x < cfgGridW; x++) {
		for (int y = 0; y < cfgGridH; y++) {
			e->brick.push_back({
				.id = id,
				.x = x,
				.y = y,
				.region = dist(grd),
			});
			id++;
		}
	}

	context::BallList =
		util::generateBall(cfgGridWF, cfgGridHF, cfgRegionNum, 3);
	for (auto &b : context::BallList) {
		region.push_back(std::make_unique<Region>(e, b));
	}

	g = new Game({
		.entity = e,
		.window = w,
		.input = in,
	});

	s = new sdl({
		.entity = e,
		.window = w,
		.input = in,
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

	loopEvent(d.input);

	if (!g->parse()) {
		stop = true;
		return;
	}

	std::random_device rd;
	std::mt19937 g(rd());

	for (int i = 0; i < cfgPhyLoop; i++) {
		std::shuffle(region.begin(), region.end(), g);
		for (auto &r : region) {
			r->update();
		}
	}

	s->render();
}
