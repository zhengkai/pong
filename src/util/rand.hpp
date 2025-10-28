#pragma once

namespace util {

inline std::mt19937 &rng() {

	static std::random_device rd;
	static std::mt19937 gen(rd());
	return gen;
}
} // namespace util
