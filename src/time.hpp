#pragma once

#include "spdlog/spdlog.h"
#include <chrono>
#include <cmath>

using namespace std::chrono;

class Time {
public:

	explicit Time(double fps = 60.0f) : start(steady_clock::now()) {
		ft = std::chrono::duration<double>(1.0 / fps);
		spdlog::info("time start, {:.0f} fps, {:.6f} s per frame", fps, ft.count());
	}

	// 每帧开始时调用
	void tick() {
		auto now = steady_clock::now();
		auto t0 = duration<double>(now - start);
		auto ti = std::chrono::floor<seconds>(t0);
		auto t = t0 - ti;
		auto idx = static_cast<int>(std::floor(t / ft));
		auto next = (idx + 1) * ft - t;
		spdlog::trace("frame = {} {:.6f}s {:.6f}s", idx, t.count(), next.count());
		std::this_thread::sleep_for(next);
	}

private:
	duration<double> ft; // 每帧目标时长 (秒)
	steady_clock::time_point start;
};
