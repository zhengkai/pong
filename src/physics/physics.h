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
	b2BodyId ballA;
	b2BodyId ballB;
	b2BodyId dot;
	PhysicsDep d;

public:
	Physics(PhysicsDep dep);
	~Physics();

	void update();

private:
	b2BodyId createBall(float x, float y);
	void createWall();
	void createDot();
	void createBrick();
	bool contactCheck(b2ShapeId *shapeId);
};
