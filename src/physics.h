#pragma once

#include "context/entity.h"
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
	b2BodyId ballBody;
	std::shared_ptr<context::Ball> ball;
	b2BodyId dot;
	PhysicsDep d;
	int region;
	std::vector<b2BodyId> brick;

public:
	Physics(PhysicsDep dep, std::shared_ptr<context::Ball> b);
	~Physics();

	void update();

private:
	void _update(float deltaTime);
	b2BodyId createBall();
	void createWall();
	void createBrick();
	bool contactCheck(b2ShapeId *shapeId);
};
