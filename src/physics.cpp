#include "physics.h"
#include "config.hpp"
#include "context/entity.h"
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>

// defualt shape def
static b2ShapeDef dsd = [] {
	b2ShapeDef sd = b2DefaultShapeDef();
	sd.material = b2DefaultSurfaceMaterial();
	sd.material.friction = 0.0f;
	sd.density = 1.0f;
	return sd;
}();

Physics::Physics(PhysicsDep dep, std::shared_ptr<context::Ball> b)
	: d(std::move(dep)), region(b->region), ball(b) {

	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = b2Vec2{0.0f, 0.0f};
	world = b2CreateWorld(&worldDef);

	createBrick();
	createWall();

	ballBody = createBall();
	// spdlog::info("phy init {} {} {}", b->region, b->pos.x, b->pos.y);
}

Physics::~Physics() {
	spdlog::trace("phy destory {}", region);
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

	int cnt = 0;
	for (auto &eb : d.entity->brick) {
		if (eb.region == region) {
			cnt++;
		}
	}

	auto power = std::max(0, std::min(cfgPowerMax, cnt) - cfgPowerMin);
	b->tone = static_cast<double>(power) / cfgPowerDiff * 10.0 + 45.0;
	// spdlog::info("brick {} {} {} {}", region, b->id, cnt, b->tone);
	return true;
}

void Physics::update() {

	float speed = d.entity->speed;
	if (speed < 1.0f) {
		_update(cfgFPSDeltaTime * speed);
	} else {
		for (int cnt = static_cast<int>(d.entity->speed); cnt > 0; cnt--) {
			_update(cfgFPSDeltaTime);
		}
	}
}

void Physics::_update(float deltaTime) {

	for (const auto &b : d.entity->brick) {
		b2BodyId bb = brick[b.id];
		if (b.region == region) {
			b2Body_Disable(bb);
		} else {
			b2Body_Enable(bb);
		}
	}

	b2World_Step(world, deltaTime, 1);
	b2ContactEvents ce = b2World_GetContactEvents(world);

	if (ce.beginCount > 0) {
		for (int i = 0; i < ce.beginCount; i++) {
			b2ContactBeginTouchEvent event = ce.beginEvents[i];
			contactCheck(&event.shapeIdA) || contactCheck(&event.shapeIdB);
		}
		// std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	b2Vec2 p = b2Body_GetPosition(ballBody);

	ball->pos = p;

	b2Vec2 v = b2Body_GetLinearVelocity(ballBody);

	if (std::abs(v.x) < 0.01f) {
		v.x *= 100.0f;
	} else if (std::abs(v.y) < 0.01f) {
		v.y *= 100.0f;
	}

	float speed = std::sqrt(v.x * v.x + v.y * v.y);

	if (speed != cfgSpeed) {
		b2Body_SetLinearVelocity(
			ballBody, b2Vec2{v.x / speed * cfgSpeed, v.y / speed * cfgSpeed});
	}

	spdlog::trace("ball {} pos = ({:10.6f},{:10.6f}), speed = {:10.6f}",
		region,
		p.x,
		p.y,
		speed);
}

b2BodyId Physics::createBall() {

	b2BodyDef ballBodyDef = b2DefaultBodyDef();
	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position = ball->pos;
	b2BodyId bb = b2CreateBody(world, &ballBodyDef);

	b2Circle circle = {
		.center = {0.0f, 0.0f},
		.radius = cfgBallRadius,
	};
	b2ShapeId ballShape = b2CreateCircleShape(bb, &dsd, &circle);
	b2Shape_SetRestitution(ballShape, 1.0f);

	b2Body_EnableContactEvents(bb, true);

	b2Body_SetBullet(bb, true);
	b2Body_SetLinearVelocity(bb, ball->speed);

	// spdlog::info("ball speed {} {}", ball->speed.x, ball->speed.y);

	return bb;
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
		bd.position = b2Vec2{b.x + 0.5f, b.y + 0.5f};
		bd.type = b2_staticBody;

		b2BodyId bb = b2CreateBody(world, &bd);

		b2Body_Disable(bb);

		brick.push_back(bb);

		b2Body_SetUserData(bb, (void *)&b);

		b2ShapeId si = b2CreatePolygonShape(bb, &dsd, &box);
		b2Shape_SetRestitution(si, 1.0f);
	}
	// spdlog::info("Physics:: brick done {}", brick.size());
}
