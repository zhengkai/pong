#pragma once

namespace context {

class Window {

public:
	float cellSize = 0.0f;
	float startX = 0.0f;
	float startY = 0.0f;
	float gridSize = 0;

public:
	Window() = default;
	virtual ~Window() = default;
};
}; // namespace context
