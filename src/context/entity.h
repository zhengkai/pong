#pragma once

#include <box2d/box2d.h>

namespace context {

struct Brick {
	int id = 0;
	int x = 0;
	int y = 0;
	int region = 0;
};

class Entity {

public:
	b2Vec2 ballA;
	b2Vec2 ballB;
	int hit = 0;
	std::vector<Brick> brick;

public:
	Entity() = default;
	virtual ~Entity() = default;
};
}; // namespace context
