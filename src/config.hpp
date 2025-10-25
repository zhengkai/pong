#pragma once

#ifdef __EMSCRIPTEN__
constexpr bool cfgWASM = true;
#else
constexpr bool cfgWASM = false;
#endif

constexpr int cfgFPS = 60;
constexpr float cfgFPSF = static_cast<float>(cfgFPS);
constexpr float cfgFPSDeltaTime = 1.0f / cfgFPSF;

constexpr float cfgBallRadius = 0.5f;

constexpr int cfgRegionNum = 6;
constexpr int cfgPhyLoop = 1;

constexpr int cfgWinW = 1600;
constexpr int cfgWinH = 960;

constexpr int cfgSpeedLevelMax = 5;

constexpr int cfgGridW = 20;
constexpr int cfgGridH = 16;

constexpr int cfgGridWF = static_cast<float>(cfgGridW);
constexpr int cfgGridHF = static_cast<float>(cfgGridH);

constexpr float cfgSpeed = 30.0f;

constexpr int cfgGridSize = (cfgWinW / cfgGridW < cfgWinH / cfgGridH)
	? cfgWinW / cfgGridW
	: cfgWinH / cfgGridH;
