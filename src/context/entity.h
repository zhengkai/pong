#pragma once

#include "brick.hpp"
#include <box2d/box2d.h>

namespace context {

struct Entity {
	int hit = 0;
	float speed = 1.0f;
	int speedLevel = 0;
	std::vector<Brick> brick;
};
}; // namespace context
