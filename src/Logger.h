#pragma once

#include <iostream>

#include <fmt/format.h>
#include <glm/glm.hpp>

enum class LogLevel {
	debug,
	info,
	error
};

extern LogLevel g_logLevel;

template <typename... T>
void log_internal(const char * file, int line, LogLevel level, fmt::format_string<T...> f, T &&... p)
{
	if(g_logLevel <= level) {
		std::string s = file;
		std::size_t found = s.find("src");
		std::string foo = s.substr(found + 4);
		
		std::cout << fmt::format("{: <36}", fmt::format("{}:{}", foo, line)) <<
		    fmt::format(f, std::forward<T>(p)...) << std::endl;
	}
}


#define log_debug(...) log_internal(__FILE__, __LINE__, LogLevel::debug, __VA_ARGS__)
#define log_info(...)  log_internal(__FILE__, __LINE__, LogLevel::info, __VA_ARGS__)
#define log_error(...) log_internal(__FILE__, __LINE__, LogLevel::error, __VA_ARGS__)


namespace fmt {

template <>
struct formatter<glm::ivec2> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext & ctx) {
		return ctx.begin();
	}
	
	template <typename FormatContext>
	auto format(const glm::ivec2 & p, FormatContext & ctx) {
		return format_to(ctx.out(), "({}, {})", p.x, p.y);
	}
};

template <>
struct formatter<glm::vec3> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext & ctx) {
		return ctx.begin();
	}
	
	template <typename FormatContext>
	auto format(const glm::vec3 & p, FormatContext & ctx) {
		return format_to(ctx.out(), "({}, {}, {})", p.x, p.y, p.z);
	}
};
} // namespace fmt
