#include "physics.h"
#include "../config.hpp"
#include "../render/layout.hpp"
#include "spdlog/spdlog.h"
#include <box2d/box2d.h>

Physics::Physics() {
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = b2Vec2{0.0f, 0.0f};
	world = b2CreateWorld(&worldDef);

	createDot();
	createWall();
	createBall();
}

Physics::~Physics() {
	b2DestroyWorld(world);
}

void Physics::update() {

	b2World_Step(world, cfgFPSDeltaTime, 8);

	Layout &layout = Layout::instance();

	layout.ball = b2Body_GetPosition(ball);
	b2Vec2 v = b2Body_GetLinearVelocity(ball);
	spdlog::info("ball pos = ({:10.6f},{:10.6f}), ({:10.6f},{:10.6f})",
		layout.ball.x,
		layout.ball.y,
		v.x,
		v.y);
}

void Physics::createBall() {

	b2BodyDef ballBodyDef = b2DefaultBodyDef();
	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position = b2Vec2{3.0f, 3.0f};
	ball = b2CreateBody(world, &ballBodyDef);

	b2ShapeDef ballShapeDef = b2DefaultShapeDef();
	ballShapeDef.material = b2DefaultSurfaceMaterial();
	ballShapeDef.material.friction = 0.0f; // 无摩擦
	ballShapeDef.density = 1.0f;

	b2Circle circle = {0};
	circle.center = b2Vec2{0.0f, 0.0f};
	circle.radius = cfgBallRadius;

	b2ShapeId ballShape = b2CreateCircleShape(ball, &ballShapeDef, &circle);
	b2Shape_SetRestitution(ballShape, 1.0f);

	b2Body_SetBullet(ball, true);
	b2Body_SetLinearVelocity(ball, b2Vec2{1.5f, 32.0f});
}

void Physics::createWall() {

	b2BodyDef bd = b2DefaultBodyDef();
	bd.type = b2_staticBody;
	b2BodyId wall = b2CreateBody(world, &bd);

	float left = 0.0f, right = static_cast<float>(cfgGridW);
	float top = 0.0f, bottom = static_cast<float>(cfgGridH);

	// 定义 shape
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.material = b2DefaultSurfaceMaterial();
	shapeDef.material.friction = 0.0f; // 无摩擦
	shapeDef.density = 0.0f;

	b2Segment seg;

	// 上边界
	seg.point1 = b2Vec2{left, top};
	seg.point2 = b2Vec2{right, top};
	b2ShapeId wallTop = b2CreateSegmentShape(wall, &shapeDef, &seg);
	b2Shape_SetRestitution(wallTop, 1.0f);

	// 下边界
	seg.point1 = b2Vec2{left, bottom};
	seg.point2 = b2Vec2{right, bottom};
	b2ShapeId wallBottom = b2CreateSegmentShape(wall, &shapeDef, &seg);
	b2Shape_SetRestitution(wallBottom, 1.0f);

	// 左边界
	seg.point1 = b2Vec2{left, bottom};
	seg.point2 = b2Vec2{left, top};
	b2ShapeId wallLeft = b2CreateSegmentShape(wall, &shapeDef, &seg);
	b2Shape_SetRestitution(wallLeft, 1.0f);

	// 右边界
	seg.point1 = b2Vec2{right, bottom};
	seg.point2 = b2Vec2{right, top};
	b2ShapeId wallRight = b2CreateSegmentShape(wall, &shapeDef, &seg);
	b2Shape_SetRestitution(wallRight, 1.0f);
}

void Physics::createDot() {

	b2BodyDef bd = b2DefaultBodyDef();
	bd.position = b2Vec2{10.0f, 12.0f};
	bd.type = b2_staticBody;

	dot = b2CreateBody(world, &bd);

	b2Polygon box = b2MakeBox(1.5f, 1.5f);

	b2ShapeDef sd = b2DefaultShapeDef();
	sd.material = b2DefaultSurfaceMaterial();
	sd.material.friction = 0.0f; // 无摩擦
	sd.density = 1.0f;

	b2ShapeId si = b2CreatePolygonShape(dot, &sd, &box);
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
