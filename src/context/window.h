#pragma once

namespace context {

struct ControlMsg {
	std::string msg;
	int expireSerial = 0;
};

class Window {

public:
	float cellSize = 0.0f;
	float startX = 0.0f;
	float startY = 0.0f;
	float gridSize = 0;
	int serial = 0;
	ControlMsg *controlMsg;

public:
	Window() = default;
	virtual ~Window() = default;
};
}; // namespace context
