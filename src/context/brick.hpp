#pragma once

#include "../config.hpp"

namespace context {

struct BrickSide {
	int n = -1;
	int s = -1;
	int e = -1;
	int w = -1;
};

struct Brick {
	int id = 0;
	float x = 0;
	float y = 0;
	int region = 0;
	double power = 0;
	BrickSide side = {};
};

inline context::Brick genBrick(int x, int y) {

	int id = x + y * config::gridW;

	context::BrickSide side;
	if (y > 0) {
		side.n = id - config::gridW;
	}
	if (y < (config::gridH - 1)) {
		side.s = id + config::gridW;
	}
	if (x > 0) {
		side.w = id - 1;
	}
	if (x < (config::gridW - 1)) {
		side.e = id + 1;
	}
	context::Brick b{
		.id = id,
		.x = static_cast<float>(x),
		.y = static_cast<float>(y),
		// .region = regionBall[dist(util::rng())],
		.region = -1,
		.power = 0,
		.side = side,
	};

	return b;
};
}; // namespace context
