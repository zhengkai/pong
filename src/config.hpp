#pragma once

#ifdef __EMSCRIPTEN__
constexpr bool cfgWASM = true;
#else
constexpr bool cfgWASM = false;
#endif

constexpr int cfgFPS = 60;
constexpr float cfgFPSF = static_cast<float>(cfgFPS);
constexpr float cfgFPSDeltaTime = 1.0f / cfgFPSF;

constexpr float cfgBallRadius = 0.57f;

constexpr int cfgRegionNum = 6;
constexpr int cfgPhyLoop = 1;

constexpr int cfgWinW = 1600;
constexpr int cfgWinH = 960;

constexpr int cfgSpeedLevelMax = 5;

constexpr int cfgGridW = 28;
constexpr int cfgGridH = 17;

constexpr int cfgBrickTotal = cfgGridW * cfgGridH;

constexpr float cfgPowerAvg =
	static_cast<float>(cfgBrickTotal) / static_cast<float>(cfgRegionNum);

constexpr int cfgPowerMin = static_cast<int>(cfgPowerAvg * 0.7f);
constexpr int cfgPowerMax = static_cast<int>(cfgPowerAvg * 1.3f);
constexpr double cfgPowerDiff = static_cast<double>(cfgPowerMax - cfgPowerMin);

constexpr float cfgGridWF = static_cast<float>(cfgGridW);
constexpr float cfgGridHF = static_cast<float>(cfgGridH);

constexpr float cfgPaddingTop = 0.8f;

constexpr float cfgSpeed = 30.0f;

constexpr int cfgGridSize = (cfgWinW / cfgGridW < cfgWinH / cfgGridH)
	? cfgWinW / cfgGridW
	: cfgWinH / cfgGridH;
