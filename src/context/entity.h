#pragma once

#include <box2d/box2d.h>

namespace context {

struct Brick {
	int id = 0;
	int x = 0;
	int y = 0;
	int region = 0;
};

struct Ball {
	int region = 0;
	b2Vec2 pos;
	b2Vec2 speed;
};

inline std::vector<Ball> BallList;

class Entity {

public:
	int hit = 0;
	std::vector<Brick> brick;

public:
	Entity() = default;
	virtual ~Entity() = default;
};
}; // namespace context
