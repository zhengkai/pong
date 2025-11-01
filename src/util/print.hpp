#pragma once

#include <string>
#include <vector>

namespace util {

std::string joinVector(
	const std::vector<int> &v, const std::string &delim = ",") {
	if (v.empty())
		return "";
	std::string result = std::to_string(v[0]);
	for (size_t i = 1; i < v.size(); ++i) {
		result += delim + std::to_string(v[i]);
	}
	return result;
};
}; // namespace util
