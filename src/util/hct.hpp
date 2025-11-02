#pragma once

#include "material/hct_solver.h"
#include "material/utils.h"
#include "rand.hpp"
#include <SDL3/SDL_pixels.h>
#include <spdlog/spdlog.h>

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

	explicit HCT(const SDL_Color c) {
		Argb argb = material_color_utilities::ArgbFromRgb(c.r, c.g, c.b);
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

	HCT Reverse() {
		return HCT(hue_ + 180.0, chroma_, tone_);
	}

	HCT Lighten(double amount) {

		double tone = tone_;
		if (amount < 0.0) {
			tone *= (1.0 + amount);
		} else {
			tone += (100.0 - tone_) * amount;
		}
		// spdlog::info("HCT lighten set: tone {:.1f} tone_ {:.1f}", tone,
		// tone_);
		return HCT(hue_, chroma_, tone);
	}

	SDL_Color ToColor() const {
		int r = material_color_utilities::RedFromInt(argb_);
		int g = material_color_utilities::GreenFromInt(argb_);
		int b = material_color_utilities::BlueFromInt(argb_);
		return SDL_Color{ static_cast<Uint8>(r),
			static_cast<Uint8>(g),
			static_cast<Uint8>(b),
			255 };
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
		// spdlog::info(
		// "HCT set: h {:.1f} c {:.1f} t {:.1f}", hue_, chroma_, tone_);
	}

	double hue_ = 0.0;
	double chroma_ = 0.0;
	double tone_ = 0.0;
	Argb argb_ = 0;
};

inline std::vector<HCT> Rainbow(int num) {

	// std::uniform_real_distribution<double> dist1(70.0, 90.0);
	// std::uniform_real_distribution<double> dist2(50.0, 70.0);

	std::vector<HCT> li;
	li.reserve(num);
	double step = 360.0 / static_cast<double>(num);
	double start =
		std::uniform_real_distribution<double>(0.0, step)(util::rng());
	for (int i = 0; i < num; ++i) {
		double hue = start + static_cast<double>(i) * step;
		li.push_back(HCT(hue, 80.0, 50.0));
	}
	return li;
}
}; // namespace util
