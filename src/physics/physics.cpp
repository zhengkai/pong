#include "physics.h"
#include "../config.hpp"
#include "spdlog/spdlog.h"
#include <box2d/box2d.h>

// defualt shape def
static b2ShapeDef dsd = [] {
	b2ShapeDef sd = b2DefaultShapeDef();
	sd.material = b2DefaultSurfaceMaterial();
	sd.material.friction = 0.0f;
	sd.density = 1.0f;
	return sd;
}();

Physics::Physics(PhysicsDep dep, context::Ball b)
	: d(std::move(dep)), region(b.region), ball(b) {

	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = b2Vec2{0.0f, 0.0f};
	world = b2CreateWorld(&worldDef);

	createBrick();
	createWall();

	ballBody = createBall(b);
	spdlog::info("phy init {} {} {}", b.region, b.pos.x, b.pos.y);
}

Physics::~Physics() {
	spdlog::info("phy destory {}", region);
	b2DestroyWorld(world);
}

bool Physics::contactCheck(b2ShapeId *shapeId) {

	auto body = b2Shape_GetBody(*shapeId);
	void *ud = b2Body_GetUserData(body);
	if (ud == nullptr) {
		return false;
	}

	context::Brick *b = (context::Brick *)ud;
	b->region = region;
	// spdlog::trace("contactCheck {}", b->id);
	return true;
}

void Physics::update() {

	for (const auto &b : d.entity->brick) {
		b2BodyId bb = brick[b.id];
		if (b.region == region) {
			b2Body_Disable(bb);
		} else {
			b2Body_Enable(bb);
		}
	}

	b2World_Step(world, cfgFPSDeltaTime, 8);
	b2ContactEvents ce = b2World_GetContactEvents(world);

	if (ce.beginCount > 0) {
		for (int i = 0; i < ce.beginCount; i++) {
			b2ContactBeginTouchEvent event = ce.beginEvents[i];
			contactCheck(&event.shapeIdA) || contactCheck(&event.shapeIdB);
		}
		// std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	b2Vec2 p = b2Body_GetPosition(ballBody);

	ball.pos = p;

	b2Vec2 v = b2Body_GetLinearVelocity(ballBody);
	spdlog::trace("ball pos = ({:10.6f},{:10.6f}), ({:10.6f},{:10.6f})",
		p.x,
		p.y,
		v.x,
		v.y);
}

b2BodyId Physics::createBall(context::Ball b) {

	b2BodyDef ballBodyDef = b2DefaultBodyDef();
	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position = b.pos;
	b2BodyId ball = b2CreateBody(world, &ballBodyDef);

	b2Circle circle = {
		.center = {0.0f, 0.0f},
		.radius = cfgBallRadius,
	};
	b2ShapeId ballShape = b2CreateCircleShape(ball, &dsd, &circle);
	b2Shape_SetRestitution(ballShape, 1.0f);

	b2Body_EnableContactEvents(ball, true);

	b2Body_SetBullet(ball, true);
	b2Body_SetLinearVelocity(ball, b.speed);

	spdlog::info("ball speed {} {}", b.speed.x, b.speed.y);

	return ball;
}

void Physics::createWall() {

	b2BodyDef bd = b2DefaultBodyDef();
	bd.type = b2_staticBody;
	b2BodyId wall = b2CreateBody(world, &bd);

	float left = 0.0f, right = static_cast<float>(cfgGridW);
	float top = 0.0f, bottom = static_cast<float>(cfgGridH);

	b2Segment seg;

	// 上边界
	seg.point1 = b2Vec2{left, top};
	seg.point2 = b2Vec2{right, top};
	b2ShapeId wallTop = b2CreateSegmentShape(wall, &dsd, &seg);
	b2Shape_SetRestitution(wallTop, 1.0f);

	// 下边界
	seg.point1 = b2Vec2{left, bottom};
	seg.point2 = b2Vec2{right, bottom};
	b2ShapeId wallBottom = b2CreateSegmentShape(wall, &dsd, &seg);
	b2Shape_SetRestitution(wallBottom, 1.0f);

	// 左边界
	seg.point1 = b2Vec2{left, bottom};
	seg.point2 = b2Vec2{left, top};
	b2ShapeId wallLeft = b2CreateSegmentShape(wall, &dsd, &seg);
	b2Shape_SetRestitution(wallLeft, 1.0f);

	// 右边界
	seg.point1 = b2Vec2{right, bottom};
	seg.point2 = b2Vec2{right, top};
	b2ShapeId wallRight = b2CreateSegmentShape(wall, &dsd, &seg);
	b2Shape_SetRestitution(wallRight, 1.0f);
}

void Physics::createBrick() {

	b2Polygon box = b2MakeBox(0.5f, 0.5f);

	for (const auto &b : d.entity->brick) {

		b2BodyDef bd = b2DefaultBodyDef();
		bd.position = b2Vec2{
			static_cast<float>(b.x) + 0.5f, static_cast<float>(b.y) + 0.5f};
		bd.type = b2_staticBody;

		b2BodyId bb = b2CreateBody(world, &bd);

		b2Body_Disable(bb);

		brick.push_back(bb);

		b2Body_SetUserData(bb, (void *)&b);

		b2ShapeId si = b2CreatePolygonShape(bb, &dsd, &box);
		b2Shape_SetRestitution(si, 1.0f);
	}
	spdlog::info("Physics:: brick done {}", brick.size());
}
