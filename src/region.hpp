#pragma once

#include "context/entity.h"
#include "physics.h"
#include <spdlog/spdlog.h>

struct Region {

private:
	int id;
	std::unique_ptr<Physics> phy;
	std::shared_ptr<context::Ball> ball;

public:
	Region(std::shared_ptr<context::Entity> e, std::shared_ptr<context::Ball> b)
		: ball(b), id(b->region) {

		phy = std::make_unique<Physics>(
			PhysicsDep{
				.entity = e,
			},
			b);
	}
	~Region() {
		spdlog::trace("region destory {}", id);
	}
	void update(float dt) {
		phy->update(dt);
	}
};
