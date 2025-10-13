#pragma once

#include "../context/entity.h"
#include <box2d/box2d.h>

struct PhysicsDep {
	std::shared_ptr<context::Entity> entity;
};

class Physics {

public:
	b2Vec2 ballPos;

private:
	b2WorldId world;
	b2BodyId ground;
	b2BodyId ball;
	b2BodyId dot;
	PhysicsDep d;

public:
	Physics(PhysicsDep dep);
	~Physics();

	void update();

private:
	void createBall();
	void createWall();
	void createDot();
};
