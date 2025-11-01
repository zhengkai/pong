#pragma once

#include "context/entity.h"
#include "context/window.h"
#include "game.h"
#include "region.hpp"
#include "sdl.h"
#include "time.hpp"

struct PongDeps {
	std::shared_ptr<context::BallCluster> ballCluster;
	std::shared_ptr<context::Entity> entity;
	std::shared_ptr<context::Window> window;
};

class Pong {

private:
	PongDeps d;
	std::unique_ptr<Time> t;
	std::unique_ptr<sdl> s;
	std::unique_ptr<Game> g;
	steady_clock::time_point prev;
	std::vector<std::unique_ptr<Region>> region;

private:
	std::vector<context::Brick> genBrick();

public:
	bool stop;
	Pong();
	~Pong();
	void loop();
	void run();
	bool init();
	bool initBrick();
};
