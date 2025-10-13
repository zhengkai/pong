#pragma once

#include <box2d/box2d.h>

namespace context {

class Entity {

public:
	b2Vec2 ballA;
	b2Vec2 ballB;
	int hit = 0;

public:
	Entity() = default;
	virtual ~Entity() = default;
};
}; // namespace context
