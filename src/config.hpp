#pragma once

#include <spdlog/spdlog.h>

namespace config {

inline bool versionOnly = false;
inline bool helpOnly = false;
inline std::string verbose = "info";

inline int regionNum;
const int regionNumDefault = 6;

const int gridWDefault = 60;
const int gridHDefault = 32;
inline int gridW;
inline int gridH;
inline float gridWF;
inline float gridHF;

inline int brickTotal;

inline float powerAvg;

inline int powerMin;
inline int powerMax;
inline double powerDiff;

inline void setGrid(int w, int h) {
	gridW = w;
	gridH = h;
	brickTotal = gridW * gridH;

	gridWF = static_cast<float>(gridW);
	gridHF = static_cast<float>(gridH);
}

inline void setRegion(int r) {
	regionNum = r;
	powerAvg = static_cast<float>(brickTotal) / static_cast<float>(regionNum);
	powerMin = static_cast<int>(powerAvg * 0.7f);
	powerMax = static_cast<int>(powerAvg * 1.3f);
	powerDiff = static_cast<double>(powerMax - powerMin);
};

}; // namespace config

#ifdef __EMSCRIPTEN__
constexpr bool cfgWASM = true;
#else
constexpr bool cfgWASM = false;
#endif

constexpr int cfgFPS = 60;
constexpr float cfgFPSF = static_cast<float>(cfgFPS);
constexpr float cfgFPSDeltaTime = 1.0f / cfgFPSF;

constexpr float cfgBallRadius = 0.57f;

constexpr int cfgPhyLoop = 1;

constexpr int cfgWinW = 1600;
constexpr int cfgWinH = 960;

constexpr int cfgSpeedLevelMax = 5;

constexpr float cfgPaddingTop = 0.8f;

constexpr float cfgSpeed = 30.0f;
