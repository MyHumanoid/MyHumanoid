#pragma once

#include <algorithm>
#include <string>

inline std::string removeExtension(const std::string& filename) {
	size_t lastdot = filename.find_last_of(".");
	if (lastdot == std::string::npos) return filename;
	return filename.substr(0, lastdot); 
}

inline std::string pathBasename(const std::string& pathname)
{
	const auto matcher = [](char c) {
		return c == '/';
	};
	
	return {
		std::find_if(pathname.rbegin(), pathname.rend(), matcher).base(),
		pathname.end()
	};
}
