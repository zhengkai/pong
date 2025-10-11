#pragma once

#include <box2d/box2d.h>

class Physics {

private:
	b2WorldId world;
	b2BodyId ground;
	b2BodyId ball;

public:
	Physics();
	~Physics();

	void update(float deltaTime);

private:
	void createWall();
};
