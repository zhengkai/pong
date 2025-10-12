#pragma once

#include <box2d/box2d.h>

class Layout {

public:
	float cellSize = 0.0f;
	float startX = 0.0f;
	float startY = 0.0f;
	float gridSize = 0;
	b2Vec2 ball;

public:
	static Layout &instance() {
		static Layout instance;
		return instance;
	}

private:
	Layout() = default;
	~Layout() = default;
	Layout(const Layout &) = delete;
	Layout &operator=(const Layout &) = delete;
};
