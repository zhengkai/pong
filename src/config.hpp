#pragma once

constexpr int cfgWinW = 1280;
constexpr int cfgWinH = 1024;

constexpr int cfgGridW = 21;
constexpr int cfgGridH = 17;

constexpr int cfgGridSize = (cfgWinW / cfgGridW < cfgWinH / cfgGridH) ? cfgWinW / cfgGridW : cfgWinH / cfgGridH;
