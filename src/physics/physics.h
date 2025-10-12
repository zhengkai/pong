#pragma once

#include <box2d/box2d.h>

class Physics {

public:
	b2Vec2 ballPos;

private:
	b2WorldId world;
	b2BodyId ground;
	b2BodyId ball;

public:
	Physics();
	~Physics();

	void update();

private:
	void createBall();
	void createWall();
};
