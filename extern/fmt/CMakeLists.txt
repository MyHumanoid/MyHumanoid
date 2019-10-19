# SPDX-License-Identifier: GPL-3.0-or-later

project(fmt)

add_library(fmt STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/src/format.cc
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/src/posix.cc
)
target_include_directories(fmt PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/include
)
