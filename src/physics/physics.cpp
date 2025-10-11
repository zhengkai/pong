#include "physics.h"
#include "../config.hpp"
#include <box2d/box2d.h>

Physics::Physics() {
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = b2Vec2{0, 0};
	world = b2CreateWorld(&worldDef);

	createWall();
}

Physics::~Physics() {
	b2DestroyWorld(world);
}

void Physics::update(float dt) {
}

void Physics::createWall() {

	b2BodyDef bd = b2DefaultBodyDef();
	bd.type = b2_staticBody;
	b2BodyId wall = b2CreateBody(world, &bd);

	float left = 0.0f, right = static_cast<float>(cfgGridW);
	float top = 0.0f, bottom = static_cast<float>(cfgGridW);

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
