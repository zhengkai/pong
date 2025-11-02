#pragma once

#include <box2d/box2d.h>

namespace context {

struct Brick {
	int id = 0;
	float x = 0;
	float y = 0;
	int region = 0;
	double power = 0;
};

struct Entity {
	int hit = 0;
	float speed = 1.0f;
	int speedLevel = 0;
	std::vector<Brick> brick;
};
}; // namespace context
