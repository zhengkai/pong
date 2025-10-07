#pragma once

#include "spdlog/spdlog.h"
#include <chrono>

class Time {
public:
	using Clock = std::chrono::steady_clock;

	explicit Time(float fps = 60.0f)
		: ft(1.0f / fps), start(Clock::now()), ac(0.0f) {

		spdlog::info("time start");
	}

	// 每帧开始时调用
	void tick() {
		using namespace std::chrono;
		// sleep1s();
		auto now = Clock::now();
		auto cost = duration<float>(now - start) * 10000;

		spdlog::info("cost = {:.6f}s", cost.count());
		auto cnt = duration_cast<milliseconds>(cost).count();
		spdlog::info("cnt = {}", cnt);
		spdlog::info("cost");
		std::this_thread::sleep_for(cost);
	}

	void sleep1s() {
		using namespace std::chrono_literals; // 支持 1s、500ms 这种字面量
		std::this_thread::sleep_for(1s);
	}

	// 获取逻辑帧步长（常量）
	float deltaTime() const {
		return ft;
	}

private:
	float ft; // 每帧目标时长 (秒)
	float ac; // 剩余时间累计
	Clock::time_point start;
};
