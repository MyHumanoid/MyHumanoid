# SPDX-License-Identifier: GPL-3.0-or-later

project(stb)

add_library(stb STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_rect_pack.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_truetype.h
	${CMAKE_CURRENT_SOURCE_DIR}/stb.cpp
)
target_include_directories(stb PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/stb
)
add_library(stb::stb ALIAS stb)
