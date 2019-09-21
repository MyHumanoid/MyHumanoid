#pragma once

#include "log/log.h"

#include <iostream>

void log(const char * msg) {
	std::cout << msg << std::endl;
}

void log_err(const char * msg) {
	std::cerr << msg << std::endl;
}
