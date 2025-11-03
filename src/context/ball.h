#pragma once

#include "../util/hct.hpp"
#include <SDL3/SDL_pixels.h>
#include <box2d/box2d.h>

namespace context {

struct Gradation {
	float x = 0.0f;
	float w = 0.0f;
	float y = 0.0f;
	float h = 0.0f;
};

struct Ball {
	b2Vec2 pos;
	b2Vec2 speed;
};

struct BallGroup {
	int region = 0;
	std::vector<std::shared_ptr<Ball>> list;
	util::HCT color = util::HCT(0, 0, 0);
	double tone = 0;
	int power = 0;
	bool hit = false;
	Gradation gradation = {};
	Gradation size = {};
};

struct BallCluster {
	std::vector<std::shared_ptr<BallGroup>> group;
};
}; // namespace context
