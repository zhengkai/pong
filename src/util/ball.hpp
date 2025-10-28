#pragma once

#include "../config.hpp"
#include "../context/entity.h"
#include "hct.hpp"
#include "rand.hpp"
#include <box2d/box2d.h>
#include <numbers>
#include <spdlog/spdlog.h>

namespace util {

inline b2Vec2 randomSpeedDirection(float speed) {
	float angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) *
		2.0f * std::numbers::pi_v<float>;
	float x = std::cos(angle) * speed;
	float y = std::sin(angle) * speed;
	return b2Vec2(x, y);
};

inline std::vector<std::shared_ptr<context::Ball>> generateBall(
	float w, float h, int count, float minDistance) {
	std::vector<std::shared_ptr<context::Ball>> li;

	std::uniform_real_distribution<float> xDist(0.0f, w - 2.0f);
	std::uniform_real_distribution<float> yDist(0.0f, h - 2.0f);

	li.reserve(count);

	auto rainbow = Rainbow(count);

	int maxAttempts = 1000; // 防止无限循环

	for (int region = 0; region < count; ++region) {
		b2Vec2 pos;
		bool validPosition = false;
		int attempts = 0;

		while (!validPosition && attempts < maxAttempts) {
			attempts++;
			pos = b2Vec2(xDist(rng()) + 1.0f, yDist(rng()) + 1.0f);
			validPosition = true;

			// 检查与已有球的距离
			for (const auto &ball : li) {
				float dx = pos.x - ball->pos.x;
				float dy = pos.y - ball->pos.y;
				float distance = std::sqrt(dx * dx + dy * dy);

				if (distance < minDistance) {
					validPosition = false;
					break;
				}
			}
		}

		if (validPosition) {
			li.push_back(std::make_shared<context::Ball>(context::Ball{
				.region = region,
				.pos = pos,
				.speed = randomSpeedDirection(cfgSpeed),
				.hue = rainbow[region],
			}));
		}
	}

	return li;
};
} // namespace util
