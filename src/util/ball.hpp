#pragma once

#include "../context/ball.h"
#include "hct.hpp"
#include "rand.hpp"
#include <box2d/box2d.h>
#include <numbers>
#include <spdlog/spdlog.h>

namespace util {

static std::uniform_real_distribution<float> angleDist(
	0.0f, 2.0f * std::numbers::pi_v<float>);

inline b2Vec2 randomSpeedDirection(float speed) {
	float angle = angleDist(rng());
	float x = std::cos(angle) * speed;
	float y = std::sin(angle) * speed;
	return b2Vec2(x, y);
};

std::vector<std::shared_ptr<context::BallGroup>> genBallGroupList(
	float w, float h, std::vector<int> numLi) {

	std::vector<std::shared_ptr<context::BallGroup>> re;
	re.reserve(numLi.size());

	std::uniform_real_distribution<float> xDist(1.0f, w - 1.0f);
	std::uniform_real_distribution<float> yDist(1.0f, h - 1.0f);
	std::uniform_real_distribution<float> speedDist(-10.0f, 10.0f);

	auto rainbow = Rainbow(numLi.size());

	int region = 0;
	for (int n : numLi) {
		n = std::max(1, std::min(n, 10));
		auto g = std::make_shared<context::BallGroup>();
		g->region = region;
		g->hue = rainbow[region];
		for (int i = 0; i < n; i++) {
			auto b = std::make_shared<context::Ball>();
			b->pos = b2Vec2(xDist(util::rng()), yDist(util::rng()));
			b->speed = b2Vec2(speedDist(util::rng()), speedDist(util::rng()));
			g->list.push_back(b);
		}
		re.push_back(g);
		region++;
	}
	return re;
};
}; // namespace util
