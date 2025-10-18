#pragma once

namespace util {
#ifdef __EMSCRIPTEN__
static std::string cfgDir = "/static";
#else
static std::string cfgDir = "/www/pong/static";
#endif

inline std::string file(const std::string filename) {
	return cfgDir + "/" + filename;
};
} // namespace util
