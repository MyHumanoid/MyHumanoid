#pragma once

#include <iostream>

#include <fmt/format.h>

template <typename... T> void log_info(T &&... p)
{
	std::cout << fmt::format(std::forward<T>(p)...) << std::endl;
}

template <typename... T> void log_error(T &&... p)
{
	std::cerr << fmt::format(std::forward<T>(p)...) << std::endl;
}
