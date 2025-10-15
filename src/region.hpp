#pragma once

#include "context/entity.h"
#include "physics/physics.h"

struct Region {

private:
	int id;
	std::unique_ptr<Physics> phy;
	context::Ball ball;

public:
	Region(std::shared_ptr<context::Entity> e, context::Ball ball)
		: ball(ball), id(ball.region) {

		phy = std::make_unique<Physics>(
			PhysicsDep{
				.entity = e,
			},
			ball);
	}
	~Region() {
		spdlog::info("region destory {}", id);
	}
	void update() {
		phy->update();
	}
};
