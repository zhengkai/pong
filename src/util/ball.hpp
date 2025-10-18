#pragma once

#include "../config.hpp"
#include "../context/entity.h"
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
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> xDist(0.0f, w - 2.0f);
	std::uniform_real_distribution<float> yDist(0.0f, h - 2.0f);

	li.reserve(count);

	int maxAttempts = 1000; // 防止无限循环

	for (int region = 0; region < count; ++region) {
		b2Vec2 pos;
		bool validPosition = false;
		int attempts = 0;

		while (!validPosition && attempts < maxAttempts) {
			attempts++;
			pos = b2Vec2(xDist(gen) + 1.0f, yDist(gen) + 1.0f);
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
			})); // 速度为0

			// spdlog::info("new ball {} pos {} {}", region, pos.x, pos.y);
		}
	}

	return li;
};
} // namespace util
