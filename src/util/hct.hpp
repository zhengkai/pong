#pragma once

#include "material/hct_solver.h"
#include "material/utils.h"
#include <SDL3/SDL_pixels.h>

namespace util {

using Argb = uint32_t;

class HCT {

public:
	HCT(double hue, double chroma, double tone) {
		SetInternalState(
			material_color_utilities::SolveToInt(hue, chroma, tone));
	}

	explicit HCT(Argb argb) {
		SetInternalState(argb);
	}

	explicit HCT(const SDL_Color sc) {
		Argb argb = material_color_utilities::ArgbFromRgb(sc.r, sc.g, sc.b);
		SetInternalState(argb);
	}

	double getHue() const {
		return hue_;
	}

	double getChroma() const {
		return chroma_;
	}

	double getTone() const {
		return tone_;
	}

	SDL_Color ToColor() const {
		int r = material_color_utilities::RedFromInt(argb_);
		int g = material_color_utilities::GreenFromInt(argb_);
		int b = material_color_utilities::BlueFromInt(argb_);
		return SDL_Color{static_cast<Uint8>(r),
			static_cast<Uint8>(g),
			static_cast<Uint8>(b),
			255};
	}

	Argb ToInt() const {
		return argb_;
	}

	void setHue(double new_hue) {
		SetInternalState(
			material_color_utilities::SolveToInt(new_hue, chroma_, tone_));
	}

	void setChroma(double new_chroma) {

		SetInternalState(
			material_color_utilities::SolveToInt(hue_, new_chroma, tone_));
	}

	void setTone(double new_tone) {
		SetInternalState(
			material_color_utilities::SolveToInt(hue_, chroma_, new_tone));
	}

private:
	void SetInternalState(Argb argb) {
		argb_ = argb;
		material_color_utilities::Cam cam =
			material_color_utilities::CamFromInt(argb);
		hue_ = cam.hue;
		chroma_ = cam.chroma;
		tone_ = material_color_utilities::LstarFromArgb(argb);
	}

	double hue_ = 0.0;
	double chroma_ = 0.0;
	double tone_ = 0.0;
	Argb argb_ = 0;
};

inline std::vector<SDL_Color> Rainbow(
	int num, double chroma = 100.0, double tone = 50.0) {

	std::vector<SDL_Color> li;
	li.reserve(num);
	double total = static_cast<double>(num + 1);
	for (int i = 0; i < num; ++i) {
		double hue = static_cast<double>(i) / total * 360.0;
		HCT hct(hue, chroma, tone);
		li.push_back(hct.ToColor());
	}
	return li;
}
}; // namespace util
