#pragma once

#include <iostream>

#include <fmt/format.h>

template <typename... T> void logger(T &&... p)
{
	std::cout << fmt::format(std::forward<T>(p)...) << std::endl;
}

template <typename... T> void logger_err(T &&... p)
{
	std::cerr << fmt::format(std::forward<T>(p)...) << std::endl;
}
