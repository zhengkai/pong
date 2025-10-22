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

inline std::vector<std::shared_ptr<Ball>> BallList;

struct Entity {
	int hit = 0;
	float speed = 1.0f;
	int speedLevel = 0;
	std::vector<Brick> brick;
};
}; // namespace context
