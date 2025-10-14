#pragma once

constexpr float cfgFPS = 60.0f;
constexpr float cfgFPSDeltaTime = 1.0f / cfgFPS;

constexpr float cfgBallRadius = 0.5f;

constexpr int cfgWinW = 2000;
constexpr int cfgWinH = 1600;

constexpr int cfgGridW = 10;
constexpr int cfgGridH = 8;

constexpr int cfgGridSize = (cfgWinW / cfgGridW < cfgWinH / cfgGridH)
	? cfgWinW / cfgGridW
	: cfgWinH / cfgGridH;
