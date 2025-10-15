#pragma once

constexpr float cfgFPS = 60.0f;
constexpr float cfgFPSDeltaTime = 1.0f / cfgFPS;

constexpr float cfgBallRadius = 0.5f;

constexpr int cfgWinW = 2000;
constexpr int cfgWinH = 1600;

constexpr int cfgGridW = 20;
constexpr int cfgGridH = 16;

constexpr int cfgGridWF = static_cast<float>(cfgGridW);
constexpr int cfgGridHF = static_cast<float>(cfgGridH);

constexpr float cfgSpeed = 30.0f;

constexpr int cfgGridSize = (cfgWinW / cfgGridW < cfgWinH / cfgGridH)
	? cfgWinW / cfgGridW
	: cfgWinH / cfgGridH;
