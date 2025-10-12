#pragma once

constexpr float cfgFPS = 10.0f;
constexpr float cfgFPSDeltaTime = 1.0f / cfgFPS;

constexpr float cfgBallRadius = 0.5f;

constexpr int cfgWinW = 1280;
constexpr int cfgWinH = 1024;

constexpr int cfgGridW = 20;
constexpr int cfgGridH = 16;

constexpr int cfgGridSize = (cfgWinW / cfgGridW < cfgWinH / cfgGridH)
	? cfgWinW / cfgGridW
	: cfgWinH / cfgGridH;
