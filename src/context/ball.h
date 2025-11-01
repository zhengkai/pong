#pragma once

#include <box2d/box2d.h>

namespace context {

struct Ball {
	b2Vec2 pos;
	b2Vec2 speed;
};

struct BallGroup {
	int region = 0;
	std::vector<std::shared_ptr<Ball>> list;
	double hue = 0;
	double tone = 0;
	int power = 0;
	bool hit = false;
};

struct BallCluster {
	std::vector<std::shared_ptr<BallGroup>> group;
};
}; // namespace context
