#pragma once

#include <iostream>

#include <fmt/format.h>

enum class LogLevel {
	debug,
	info,
	error
};

extern LogLevel g_logLevel;

template <typename... T> void log_debug(T &&... p)
{
	if(g_logLevel <= LogLevel::debug) {
		std::cout << fmt::format(std::forward<T>(p)...) << std::endl;
	}
}

template <typename... T> void log_info(T &&... p)
{
	if(g_logLevel <= LogLevel::info) {
		std::cout << fmt::format(std::forward<T>(p)...) << std::endl;
	}
}

template <typename... T> void log_error(T &&... p)
{
	if(g_logLevel <= LogLevel::error) {
		std::cerr << fmt::format(std::forward<T>(p)...) << std::endl;
	}
}
