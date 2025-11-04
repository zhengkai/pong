#pragma once

#include "../config.hpp"
#include <filesystem>
#include <string>
#include <unistd.h>

namespace fs = std::filesystem;

namespace util {

inline std::string file(const std::filesystem::path in) {
	for (const auto &dir : config::dirList) {
		auto fullPath = dir / in;
		if (fs::exists(fullPath)) {
			return fullPath.string();
		}
	}
	auto s = in.string();
	spdlog::critical("File not found: {}", s);
	return s;
};

inline fs::path getAppDir() {

#if _MSC_VER
	wchar_t buf[MAX_PATH];
	GetModuleFileNameW(nullptr, buf, MAX_PATH);
	return fs::path(buf).parent_path();
#elif __APPLE__
	char buf[1024];
	uint32_t size = sizeof(buf);
	_NSGetExecutablePath(buf, &size);
	return fs::path(buf).parent_path();
#else
	char buf[1024];
	ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
	buf[len] = '\0';
	return fs::path(buf).parent_path();
#endif
};

inline void InitDir() {
	if (!config::dirList.empty()) {
		return;
	}
#ifdef __EMSCRIPTEN__
	config::dirList.push_back(fs::path("/static"));
	return;
#endif
	auto appDir = getAppDir();

	const char *envDir = std::getenv("PONG_DIR");
	if (envDir) {
		auto pDir = fs::path(envDir);
		if (pDir != appDir) {
			config::dirList.push_back(pDir);
		}
	}
	config::dirList.push_back(appDir);
};
} // namespace util
