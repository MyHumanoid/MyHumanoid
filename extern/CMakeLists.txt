# SPDX-License-Identifier: GPL-3.0-or-later

add_subdirectory(fmt)
add_subdirectory(imgui)
add_subdirectory(simpleini)
add_subdirectory(nativefiledialog)

project(foo)

# ============================================================

add_library(stb_image INTERFACE)
target_sources(stb_image INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.h
)

target_include_directories(stb_image INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/stb
)



# ============================================================


