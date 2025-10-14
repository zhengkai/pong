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

Physics::Physics(PhysicsDep dep, int region)
	: d(std::move(dep)), region(region) {
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = b2Vec2{0.0f, 0.0f};
	world = b2CreateWorld(&worldDef);

	// createDot();
	createBrick();
	createWall();

	float y = static_cast<float>(std::rand()) / RAND_MAX *
		static_cast<float>(cfgGridH);

	ballA =
		createBall(region == 1 ? static_cast<float>(cfgGridW - 2) : 2.0f, y);
	// b2Body_SetUserData(ballA, (void *)7);
	// ballB = createBall(6.0f, 6.0f);
	// b2Body_SetUserData(ballB, (void *)8);
}

Physics::~Physics() {
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

	b2Vec2 p = b2Body_GetPosition(ballA);
	if (region == 1) {
		d.entity->ballA = p;
	} else {
		d.entity->ballB = p;
	}
	b2Vec2 v = b2Body_GetLinearVelocity(ballA);
	spdlog::trace("ball pos = ({:10.6f},{:10.6f}), ({:10.6f},{:10.6f})",
		p.x,
		p.y,
		v.x,
		v.y);
	// d.entity->ballB = b2Body_GetPosition(ballB);
}

b2BodyId Physics::createBall(float x, float y) {

	b2BodyDef ballBodyDef = b2DefaultBodyDef();
	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position = b2Vec2{x, y};
	b2BodyId ball = b2CreateBody(world, &ballBodyDef);

	b2Circle circle = {
		.center = {0.0f, 0.0f},
		.radius = cfgBallRadius,
	};
	b2ShapeId ballShape = b2CreateCircleShape(ball, &dsd, &circle);
	b2Shape_SetRestitution(ballShape, 1.0f);

	b2Body_EnableContactEvents(ball, true);

	b2Body_SetBullet(ball, true);
	b2Body_SetLinearVelocity(ball, b2Vec2{11.5f, 32.0f});

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

		brick.push_back(bb);

		b2Body_SetUserData(bb, (void *)&b);

		if (b.region == 0) {
			b2Body_Disable(bb);
		}

		b2ShapeId si = b2CreatePolygonShape(bb, &dsd, &box);
		b2Shape_SetRestitution(si, 1.0f);
	}
}

void Physics::createDot() {

	b2BodyDef bd = b2DefaultBodyDef();
	bd.position = b2Vec2{10.0f, 12.0f};
	bd.type = b2_staticBody;

	dot = b2CreateBody(world, &bd);

	// b2Body_SetUserData(dot, (void *)9);

	b2Polygon box = b2MakeBox(1.5f, 1.5f);

	b2ShapeId si = b2CreatePolygonShape(dot, &dsd, &box);
	b2Shape_SetRestitution(si, 1.0f);
}

// void Physics::createDot() {
//
// 	b2BodyDef ballBodyDef = b2DefaultBodyDef();
// 	ballBodyDef.type = b2_staticBody;
// 	ballBodyDef.position = b2Vec2{8.0f, 8.0f};
// 	dot = b2CreateBody(world, &ballBodyDef);
//
// 	b2ShapeDef ballShapeDef = b2DefaultShapeDef();
// 	ballShapeDef.material = b2DefaultSurfaceMaterial();
// 	ballShapeDef.material.friction = 0.0f; // 无摩擦
// 	ballShapeDef.density = 1.0f;
//
// 	b2Circle circle = {0};
// 	circle.center = b2Vec2{0.0f, 0.0f};
// 	circle.radius = cfgBallRadius * 3;
//
// 	b2ShapeId ballShape = b2CreateCircleShape(dot, &ballShapeDef, &circle);
// 	b2Shape_SetRestitution(dot, 1.0f);
// }
